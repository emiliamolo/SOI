#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "lock.h"

#define sleep(n) usleep(1000*(n))

#define M 5
#define N 5
#define ARRLEN 10240

rwlock_t* rwlock;

int arr[ARRLEN];

void * escritor(void *arg)
{
    int i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 100);

        rwlock_write_lock(rwlock);

        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++)
            arr[i] = num;

        rwlock_write_unlock(rwlock);
    }
    return NULL;
}
void * lector(void *arg)
{
    int v, i;
    int num = arg - (void*)0;
    while (1) {
        sleep(random() % 100);

        rwlock_read_lock(rwlock);

        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
            if (arr[i] != v)
                break;
        }

        if (i < ARRLEN)
            printf("Lector %d, error de lectura\n", num);
        else
            printf("Lector %d, dato %d\n", num, v);

        rwlock_read_unlock(rwlock);
    }
    return NULL;
}

int main()
{
    pthread_t lectores[M], escritores[N];
    int i;

    rwlock = rwlock_init();

    for (i = 0; i < M; i++)
        pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    for (i = 0; i < N; i++)
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);

    pthread_join(lectores[0], NULL); /* Espera para siempre */

    rwlock_destroy(rwlock);

    return 0;
}