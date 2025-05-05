#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 5
#define N 5
#define SZ 8

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t lugares_libres_sem;
sem_t lugares_ocupados_sem;
/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];
int num = 0;
void enviar(int *p)
{
    sem_wait(&lugares_libres_sem);
    pthread_mutex_lock(&mutex);
    buffer[num] = p;
    num++;
	pthread_mutex_unlock(&mutex);
    sem_post(&lugares_ocupados_sem);
}

int* recibir()
{
    sem_wait(&lugares_ocupados_sem);
    pthread_mutex_lock(&mutex);
    int* ret = buffer[num-1];
    num--;
	pthread_mutex_unlock(&mutex);
    sem_post(&lugares_libres_sem);
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
    sem_init(&lugares_libres_sem, 0, SZ); // 0 para threads, 1 para procesos
    sem_init(&lugares_ocupados_sem, 0, 0);

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
