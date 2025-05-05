#include <pthread.h>

#include "lock.h"

int rwlock_init(rwlock_t* lock) {
    lock->readers = 0;
    return pthread_mutex_init(&lock->lock, NULL);
}

int rwlock_read_lock(rwlock_t* lock) {
    if (lock->readers > 0) {
        lock->readers++; // TODO: Lock?
        return 0;
    }
    if (!pthread_mutex_lock(&lock->lock)) {
        lock->readers = 1;
        return 0;
    }
    return -1;
}

int rwlock_read_unlock(rwlock_t*);

int rwlock_write_lock(rwlock_t*);

int rwlock_write_unlock(rwlock_t*);