#include <pthread.h>
#include <stdlib.h>

#include "lock.h"

rwlock_t* rwlock_init() {
    rwlock_t* lock = malloc(sizeof(rwlock_t));
    pthread_mutex_init(&lock->lock, NULL);
    pthread_mutex_init(&lock->lockr, NULL);
    pthread_mutex_init(&lock->lockw, NULL);
    lock->readers = 0;
    return lock;
}

void rwlock_read_lock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lockw); // Espera hasta que termine un writer (o bien, otro read lock, pero sería mala suerte)
    pthread_mutex_lock(&lock->lockr);
    if (lock->readers == 0)
        pthread_mutex_lock(&lock->lock); // Bloquea directamente, porque si lock->readers es 0, entonces si etá bloqueado sólo podría ser writer, que no puede porque tiene a lockw
    lock->readers++;
    pthread_mutex_unlock(&lock->lockr);
    pthread_mutex_unlock(&lock->lockw);
}

void rwlock_read_unlock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lockr);
    lock->readers--;
    if (lock->readers == 0)
        pthread_mutex_unlock(&lock->lock);
    pthread_mutex_unlock(&lock->lockr);
}

void rwlock_write_lock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lockw);
    pthread_mutex_lock(&lock->lock);
}

void rwlock_write_unlock(rwlock_t* lock) {  
    pthread_mutex_unlock(&lock->lock);
    pthread_mutex_unlock(&lock->lockw);
}

void rwlock_destroy(rwlock_t* lock) {
    pthread_mutex_destroy(&lock->lock);
    pthread_mutex_destroy(&lock->lockr);
    pthread_mutex_destroy(&lock->lockw);
    free(lock);
}

/*
En este caso, si un escritor quiere escribir mientras hay lectores, se bloquea inicialmente el mutex sobre la variable de lectores de forma tal que
cualquier lector nuevo que quiera entrar no pueda hasta que se desbloquee, que sólo será posible cuando se desbloquee el mutex al irse el escritor.
De esta forma, en vez de dar pasos a lectores nuevos, se espera a que éstos terminen, se le cede lugar al escritor que tomó el lock del mutex
de la cantidad de lectores, y finalmente se desbloquea (termina el escritor), dando lugar a un nuevo lector o escritor.
*/