#include "concurrent_queue.h"
#include "cola.h"

void concurrent_queue_init(ConcurrentQueue* q) {
    q->q = cola_crear();
    pthread_mutex_init(&q->mutex, NULL);
}

int concurrent_isEmpty(ConcurrentQueue* q) {
    pthread_mutex_lock(&q->mutex);
    int ret = cola_es_vacia(q->q);
    pthread_mutex_unlock(&q->mutex);
    return ret;
}

void concurrent_push(ConcurrentQueue* q, void* val, FuncionCopia copy) {
    pthread_mutex_lock(&q->mutex);
    q->q = cola_encolar(q->q, val, copy);
    pthread_mutex_unlock(&q->mutex);
}

void concurrent_pop(ConcurrentQueue* q, FuncionDestructora destroy) {
    pthread_mutex_lock(&q->mutex);
    q->q = cola_desencolar(q->q, destroy);
    pthread_mutex_unlock(&q->mutex);
}

void* concurrent_top(ConcurrentQueue* q) {
    pthread_mutex_lock(&q->mutex);
    void* val = cola_inicio(q->q);
    pthread_mutex_unlock(&q->mutex);
    return val;
}

void concurrent_destroy(ConcurrentQueue* q, FuncionDestructora destroy) {
    pthread_mutex_lock(&q->mutex);
    cola_destruir(q->q, destroy);
    pthread_mutex_unlock(&q->mutex);
}