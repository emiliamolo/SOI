#include <pthread.h>
#include "semaphore.h"

void semaphore_init(semaphore_t *s, int n) {
    pthread_mutex_init(&s->mutex, NULL);
    pthread_cond_init(&s->hay_lugar, NULL);
    s->n = n;
}

void semaphore_destroy(semaphore_t *s) {
    pthread_mutex_destroy(&s->mutex);
    pthread_cond_destroy(&s->hay_lugar);
}

void semaphore_wait(semaphore_t *s) {
    pthread_mutex_lock(&s->mutex);
    while (s->n == 0)
        pthread_cond_wait(&s->hay_lugar, &s->mutex);
    
    s->n--;
    pthread_mutex_unlock(&s->mutex);
}

void semaphore_post(semaphore_t *s) {
    pthread_mutex_lock(&s->mutex);
    
    s->n++;
    pthread_cond_signal(&s->hay_lugar);
    
    pthread_mutex_unlock(&s->mutex);
}