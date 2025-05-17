#include <semaphore.h>
#include <pthread.h>
#include "conditional.h"

void cond_init(cond_t *cond){
    sem_init(&cond->cond, 0, 1);
}
void cond_destroy(cond_t *cond) {
    sem_destroy(&cond->cond);
}

void cond_wait(cond_t *cond, pthread_mutex_t *mutex){
    pthread_mutex_unlock(mutex);
    sem_wait(&cond->cond);
    pthread_mutex_lock(mutex);
}

void cond_signal(cond_t *cond) {
    sem_post(&cond->cond);
}