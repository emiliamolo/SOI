#ifndef __BARRIER_H__
#define __BARRIER_H__

#include <pthread.h>

typedef struct {
    int locked,unlocked,n;
    pthread_spinlock_t mutex;
} barrier;

void barrier_init(barrier *b, int n);
void barrier_wait(barrier *b);

#endif /* __BARRIER_H__ */