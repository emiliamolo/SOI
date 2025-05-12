#include <pthread.h>
#include <stdlib.h>

#include "rwlock.h"

rwlock_t* rwlock_init() {
    rwlock_t* lock = malloc(sizeof(rwlock_t));
    pthread_mutex_init(&lock->lock, NULL);
    pthread_mutex_init(&lock->lockr, NULL);
    lock->readers = 0;
    return lock;
}

void rwlock_read_lock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lockr);
    if (lock->readers == 0)
        pthread_mutex_lock(&lock->lock);
    lock->readers++;
    pthread_mutex_unlock(&lock->lockr);
}

void rwlock_read_unlock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lockr);
    lock->readers--;
    if (lock->readers == 0)
        pthread_mutex_unlock(&lock->lock);
    pthread_mutex_unlock(&lock->lockr);
}

void rwlock_write_lock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lock);
}

void rwlock_write_unlock(rwlock_t* lock) {
    pthread_mutex_unlock(&lock->lock);
}

void rwlock_destroy(rwlock_t* lock) {
    pthread_mutex_destroy(&lock->lock);
    pthread_mutex_destroy(&lock->lockr);
    free(lock);
}

/*
En este caso puede haber starvation de los escritores, ya que mientras haya algún lector leyendo, no se le va a dar lugar a los escritores.
Entonces, es posible que a la vez que se van lectores entren nuevos, causando que ningún escritor pueda escribir hasta que todos los lectores
terminen.
*/