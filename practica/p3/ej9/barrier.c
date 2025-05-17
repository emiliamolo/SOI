#include <pthread.h>
#include "barrier.h"

void barrier_init(barrier *b, int n){
    pthread_spin_init(&b->mutex, PTHREAD_PROCESS_PRIVATE);
    b->n = n;
    b->unlocked = 0;
    b->locked = 0;
}

void barrier_wait(barrier *b) {
    int restarting = 1;
    while (restarting){
        pthread_spin_lock(&b->mutex);
        if (b->locked < b->n)
            restarting = 0;
        else pthread_spin_unlock(&b->mutex);
    }

    b->locked++;
    pthread_spin_unlock(&b->mutex);

    int waiting = 1;
    while (waiting) {
        pthread_spin_lock(&b->mutex);
        if (b->locked == b->n) 
            waiting = 0;
        else pthread_spin_unlock(&b->mutex);
    }
    b->unlocked++;
    if (b->unlocked == b->n) 
        b->locked = b->unlocked = 0;

    pthread_spin_unlock(&b->mutex);
}