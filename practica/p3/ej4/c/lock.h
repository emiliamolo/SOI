#ifndef __LOCK_H__
#define __LOCK_H__

#include <pthread.h>
#include "cola.h"

typedef struct {
    pthread_cond_t istop;
    int nxt;
} QueueNode;

typedef struct {
    Cola q;
    pthread_mutex_t lock,mLock;
    int readers;
} rwlock_t;

rwlock_t* rwlock_init();

void rwlock_read_lock(rwlock_t*);

void rwlock_read_unlock(rwlock_t*);

void rwlock_write_lock(rwlock_t*);

void rwlock_write_unlock(rwlock_t*);

void rwlock_destroy(rwlock_t*);

#endif