#ifndef __CONCURRENT_QUEUE_H__
#define __CONCURRENT_QUEUE_H__

#include <pthread.h>
#include "cola.h"

typedef struct {
    Cola q;
    pthread_mutex_t mutex;
} ConcurrentQueue;

void concurrent_queue_init(ConcurrentQueue*);

int concurrent_isEmpty(ConcurrentQueue*);

void concurrent_push(ConcurrentQueue*, void*, FuncionCopia);

void concurrent_pop(ConcurrentQueue*, FuncionDestructora);

void* concurrent_top(ConcurrentQueue*);

void concurrent_destroy(ConcurrentQueue*, FuncionDestructora);

#endif /*  __CONCURRENT_QUEUE_H__ */