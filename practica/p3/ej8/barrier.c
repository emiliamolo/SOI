#include <pthread.h>
#include "barrier.h"

void barrier_init(barrier *b, int n){
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->proceed, NULL);
    pthread_cond_init(&b->restarted, NULL);
    b->n = n;
    b->unlocked = 0;
    b->locked = 0;
}

void barrier_wait(barrier *b) {
    pthread_mutex_lock(&b->mutex);
    while (b->locked == b->n)
        pthread_cond_wait(&b->restarted, &b->mutex);

    b->locked++;
    while (b->locked < b->n) {
        pthread_cond_wait(&b->proceed, &b->mutex);
    }
    pthread_cond_broadcast(&b->proceed);
    b->unlocked++;

    if (b->unlocked == b->n) {
        b->locked = b->unlocked = 0;
        pthread_cond_broadcast(&b->restarted);
    }
    pthread_mutex_unlock(&b->mutex);
}