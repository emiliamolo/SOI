#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
#include <math.h>
#include "common.h"
#include "ll.h"
#include "kv.h"
#include "hash.h"



struct bin {
	/* Una lista de struct kv, sin orden en particular */
	struct list vals;
};

static int TableSZ;
static int mutexesSZ;

static pthread_mutex_t *mutexes;
static struct bin *Table;

static pthread_spinlock_t spinlockLRU;
static struct list LRU = LIST_INIT(LRU);

long kv_keys()
{
	long r = 0;
	int i,j;
	/* Recorremos todos los bins contando cuantos pares KV tenemos */

	for (i = 0; i < mutexesSZ; i++) {
		pthread_mutex_lock(&mutexes[i]);

		for (j = i; j < TableSZ; j += mutexesSZ) {
			struct list *p;
			list_for_each(p, &Table[j].vals)
				r++;
		}

		pthread_mutex_unlock(&mutexes[i]);
	}

	return r;
}

static int binof(u64 hash)
{
	return hash % TableSZ;
}

/* Refcounting */
static void kv_free(struct kv *kv)
{
	/* El nodo YA DEBE estar deslinkeado de ambas listas */
	assert(kv->lru.next == NULL);
	assert(kv->lru.prev == NULL);
	assert(kv->list.next == NULL);
	assert(kv->list.prev == NULL);

	/* log(0, "free"); */
	free(kv->v);
	free(kv->k);
	free(kv);
}


static inline void kv_grab(struct kv *kv)
{
	kv->nrefs++;
}


void __kv_release(struct kv *kv)
{
	assert (kv->nrefs > 0); 
	kv->nrefs--;
	if (kv->nrefs == 0) {
		/*
		 * Si nrefs == 0, no puede estar en la tabla
		 * ni LRU, así que es seguro liberarlo.
		 */
		kv_free(kv);
	}
}

void kv_release(struct kv *kv)
{
	int bin = kv->bin; 
	pthread_mutex_lock(&mutexes[bin % mutexesSZ]);
	__kv_release(kv);
	pthread_mutex_unlock(&mutexes[bin % mutexesSZ]);

}
/* / Refcounting */

/* Retorna una cota inferior de los bytes liberados */
static int kv_evict1()
{
	struct list *p;
	struct kv *kv;
	int bin;

	/*
	 * Nota: hacemos trylock dentro del bucle para evitar
	 * un posible deadlock, ya que hay un ciclo en el
	 * orden de los recursos.
	 */

	log(2, "EVICT1 comenzando");

	pthread_spin_lock(&spinlockLRU);
	for (p = LRU.prev; p != &LRU; p = p->prev) {
		kv = list_entry(p, struct kv, lru);
		bin = kv->bin;

		/* Intentar tomar lock, si falla seguimos */
		if (pthread_mutex_trylock(&mutexes[bin % mutexesSZ])){
			continue;
		}

		/*
		 * Si está tomado por alguien (aparte de la misma
		 * tabla), lo ignoramos
		 */
		if (kv->nrefs > 1) {
			pthread_mutex_unlock(&mutexes[bin % mutexesSZ]);
			continue;
		}


		/* Si no, listo, liberamos este. Salimos con lock tomado */
		break;
	}

	/* Si entramos acá, no encontramos nada, y no tenemos ningún lock. */
	if (p == &LRU) {
		log(1, "LRU vacía!?!?");
		pthread_spin_unlock(&spinlockLRU);
		return 0;
	}

	int r = kv->klen + kv->vlen;

	/* Lo sacamos de la LRU y del mapa, y soltamos la ref de la tabla. */
	list_unlink(&kv->lru);
	pthread_spin_unlock(&spinlockLRU);

	list_unlink(&kv->list);
	__kv_release(kv);
	pthread_mutex_unlock(&mutexes[bin % mutexesSZ]);
	return r;
}

void * galloc(size_t size)
{
	void * ret;
	int laps = 0;

	ret = malloc(size);
	while (!ret && laps < 10) {
		kv_evict1();
		ret = malloc(size);
		laps++;
	}

	if (!ret)
		log(2, "galloc no anduvo!!");

	return ret;
}

void * memdup(const void *p, int len)
{
	void *r = galloc(len);
	if (!r) {
		log(2, "memdup OOM");
		return NULL;
	}
	memcpy(r, p, len);
	return r;
}

static bool kmatch(int klen1, const char *k1, int klen2, const char *k2)
{
	if (klen1 != klen2)
		return false;

	return (memcmp(k1, k2, klen1) == 0);
}

struct kv * __kv_lookup(u64 hash, int idx, int klen, const char *k)
{
	struct list *p;

	list_for_each(p, &Table[idx].vals) {
		struct kv *kv = list_entry(p, struct kv, list);
		/* log(2, "loop k = %s", kv->k); */
		if (hash != kv->hash)
			continue;

		if (kmatch(klen, k, kv->klen, kv->k))
			return kv;
	}

	return NULL;
}


static void bump(struct kv *kv)
{
	assert(kv);
	struct list *l = &kv->lru;

	assert(l->next);
	assert(l->prev);
	list_unlink(l);
	list_add_head(&LRU, l);

}

struct kv * kv_lookup(int klen, const char *k)
{
	u64 hash = hash_buf(klen, k);
	int idx = binof(hash);

	pthread_mutex_lock(&mutexes[idx % mutexesSZ]);
	struct kv * ret = __kv_lookup(hash, idx, klen, k);	
	if (ret) {
		kv_grab(ret);

		pthread_spin_lock(&spinlockLRU);
		bump(ret);
		pthread_spin_unlock(&spinlockLRU);
	}
	pthread_mutex_unlock(&mutexes[idx % mutexesSZ]);

	return ret;
}

/* devuelve .list y .lru sin inicializar */
static
struct kv * new_node(u64 hash, int idx, int klen, char *k, int vlen,
		     char *v)
{
	/* No estaba, crear e insertar */
	struct kv *node = galloc(sizeof *node);
	if (!node)
		return NULL;

	node->hash   = hash;
	node->bin    = idx;
	node->klen   = klen;
	node->k      = k;
	node->vlen   = vlen;
	node->v      = v;
	node->nrefs  = 0;

	return node;
}

int kv_add(int klen, char *k, int vlen, char *v)
{
	assert(k);
	assert(v);
	u64 hash = hash_buf(klen, k);
	int idx = binof(hash);

	/*
	 * Siempre creamos un nodo nuevo, borramos el nodo existente si
	 * lo había, y luego insertamos.
	 *
	 * Por qué esta lógica? Es más facil por el conteo de referencias...
	 * porque para cambiarle el valor al nodo actual, tendríamos que
	 * sobreescribir kv->v, y liberar el viejo, pero el mismo puede estar
	 * siendo usado. En vez de agregar *otro* conteo de referencias para
	 * kv->v, simplemente hacemos un nodo KV nuevo.
	 */

	struct kv *new = new_node(hash, idx, klen, k, vlen, v);
	if (!new) {
		log(2, "kv_add oom");
		return -1;
	}
	kv_grab(new);

	pthread_mutex_lock(&mutexes[idx % mutexesSZ]);
	struct kv *old = __kv_lookup(hash, idx, klen, k);

	/* Si existía algo en esta clave, lo removemos. */
	if (old) {
		list_unlink(&old->list);

		pthread_spin_lock(&spinlockLRU);
		list_unlink(&old->lru);
		pthread_spin_unlock(&spinlockLRU);

		__kv_release(old);
	}

	list_add_head(&Table[idx].vals, &new->list);
	//pthread_mutex_unlock(&mutexes[idx % mutexesSZ]); // antes el mutex estaba aca y no andaba!

	pthread_spin_lock(&spinlockLRU);
	list_add_head(&LRU, &new->lru);
	pthread_spin_unlock(&spinlockLRU);
	pthread_mutex_unlock(&mutexes[idx % mutexesSZ]); // ahora aca si anda, si no otro hilo lo puede cambiar cuando aun 
													// no esta correctamente enlazado en ambas listas.
	
	return 0;
}


/*
 * No hay free! Sólo saca el elemento del mapa. El llamante
 * debe usar eventualmente kv_release para borrarlo.
 */
struct kv * kv_take(int klen, const char *k)
{
	u64 hash = hash_buf(klen, k);
	int idx = binof(hash);
	
	pthread_mutex_lock(&mutexes[idx % mutexesSZ]);
	struct kv * ret = __kv_lookup(hash, idx, klen, k);
	if (ret) {
		/* Quitar de la tabla */

		list_unlink(&ret->list);
		pthread_mutex_unlock(&mutexes[idx % mutexesSZ]);

		pthread_spin_lock(&spinlockLRU);
		list_unlink(&ret->lru);
		pthread_spin_unlock(&spinlockLRU);
	
		/*
		 * Nota: no tocamos el refcount. Canjeamos la referencia
		 * desde la tabla por la referencia desde el código cliente,
		 * quedando el refcount igual.
		 */
	}
	else pthread_mutex_unlock(&mutexes[idx % mutexesSZ]);

	return ret;
}
bool kv_remove(int klen, const char *k)
{
	/* Quitar y soltar, nada más */
	struct kv *kv = kv_take(klen, k);
	if (kv) {
		kv_release(kv);
		return true;
	} else {
		return false;
	}
}

void kv_clear_all()
{
	struct list *p, *swap;
	int i,j;

	log(3, "CLEAR ALL");

	for (i = 0; i < mutexesSZ; i++) {
		pthread_mutex_lock(&mutexes[i]);

		for (j = i; j < TableSZ; j += i)
			list_for_each_safe(p, swap, &Table[j].vals) {
				struct kv *kv = list_entry(p, struct kv, list);
				list_unlink(&kv->list);

				pthread_spin_lock(&spinlockLRU); // TODO: O hago lock desde antes? Y está bien por secciones para la tabla?
				list_unlink(&kv->lru);
				pthread_spin_unlock(&spinlockLRU);

				__kv_release(kv);
			}
		
		pthread_mutex_unlock(&mutexes[i]);
	}
}

int kv_init(int tablesz)
{
	int i;

	TableSZ = tablesz;
	Table = malloc(TableSZ * sizeof Table[0]);
	if (!Table)
		quit("malloc kv_init");

	for (i = 0; i < TableSZ; i++)
		list_init(&Table[i].vals);


	/*INICIALIZAR LOCKS*/
	mutexesSZ = sqrt(tablesz);
	mutexes = malloc(sizeof(pthread_mutex_t)*mutexesSZ);
	for (i=0;i<mutexesSZ;i++)
		pthread_mutex_init(&mutexes[i], NULL);

	pthread_spin_init(&spinlockLRU, PTHREAD_PROCESS_PRIVATE);

	return 0;
}

int kv_finish()
{
	free(Table);
	free(mutexes);
	return 0;
}
