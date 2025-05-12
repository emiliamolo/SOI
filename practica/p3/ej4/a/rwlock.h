#ifndef __RWLOCK_H__
#define __RWLOCK_H__

#include <pthread.h>

typedef struct {
    pthread_mutex_t lock, lockr;
    int readers;
} rwlock_t;

rwlock_t* rwlock_init();

void rwlock_read_lock(rwlock_t*);

void rwlock_read_unlock(rwlock_t*);

void rwlock_write_lock(rwlock_t*);

void rwlock_write_unlock(rwlock_t*);

void rwlock_destroy(rwlock_t*);

#endif