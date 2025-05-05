#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 5
#define N 5
#define SZ 8

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_hay_lugar = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_hay_elemento = PTHREAD_COND_INITIALIZER;

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];
int num = 0, posi = 0, posf = 0;

void enviar(int *p)
{
	pthread_mutex_lock(&mutex);
	while(num == SZ)
		pthread_cond_wait(&cond_hay_lugar, &mutex);
	buffer[posf] = p;
	++num;
	posf = (posf+1) % SZ;
	pthread_cond_signal(&cond_hay_elemento);
	pthread_mutex_unlock(&mutex);
}

int* recibir()
{
	pthread_mutex_lock(&mutex);
	while (num == 0)
		pthread_cond_wait(&cond_hay_elemento, &mutex);
	int *ret = buffer[posi];
	posi = (posi+1)%SZ;
	--num;
	pthread_cond_signal(&cond_hay_lugar);
	pthread_mutex_unlock(&mutex);
	return ret;
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);

		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);

		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
	}
	return NULL;
}

int main()
{
	pthread_t productores[M], consumidores[N];
	int i;

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
