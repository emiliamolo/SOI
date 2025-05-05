#ifndef __LOCK_H__

#include <pthread.h>

typedef struct {
    pthread_mutex_t lock;
    int readers = 0; // 0 if unlocked, positive if locked with read, -1 is locked with write
} rwlock_t;

int rwlock_init(rwlock_t*);

int rwlock_read_lock(rwlock_t*);

int rwlock_read_unlock(rwlock_t*);

int rwlock_write_lock(rwlock_t*);

int rwlock_write_unlock(rwlock_t*);

#endif __LOCK_H__