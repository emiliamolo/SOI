#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t hay_lugar;
    int n;
} semaphore_t;

void semaphore_init(semaphore_t *s, int n);
void semaphore_destroy(semaphore_t *s);

void semaphore_wait(semaphore_t *s);

void semaphore_post(semaphore_t *s);

#endif /* __SEMAPHORE_H__ */