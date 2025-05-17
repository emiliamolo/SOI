#ifndef __CONDITIONAL_H__
#define __CONDITIONAL_H__

#include <pthread.h>
#include <semaphore.h>

typedef struct {
    sem_t cond;
} cond_t;

void cond_init(cond_t *cond);
void cond_destroy(cond_t *cond);

void cond_wait(cond_t *cond, pthread_mutex_t *mutex);
void cond_signal(cond_t *cond);


#endif /* __ CONDITIONAL_H__ */