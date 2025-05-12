#include <pthread.h>
#include <stdlib.h>

#include "lock.h"

QueueNode* nodeCpy(QueueNode* node) {
    return node;
}

void nodeDestroy(QueueNode* node) {
    pthread_cond_destroy(&node->istop);
    free(node);
}

void _lock(rwlock_t* lock) {
    QueueNode* mNode = malloc(sizeof(QueueNode));
    mNode->nxt = 0;
    pthread_cond_init(&mNode->istop, NULL);

    if (cola_es_vacia(lock->q))
        mNode->nxt = 1;

    lock->q = cola_encolar(lock->q, mNode, (FuncionCopia)nodeCpy);

    while(!mNode->nxt) {
        pthread_cond_wait(&mNode->istop, &lock->lock);
    }
}

void _unlock(rwlock_t* lock) {
    if (!cola_es_vacia(lock->q)) {
        QueueNode *node = cola_inicio(lock->q);
        node->nxt = 1;
        pthread_cond_signal(&node->istop);
    }

}

rwlock_t* rwlock_init() {
    rwlock_t* lock = malloc(sizeof(rwlock_t));
    pthread_mutex_init(&lock->lock, NULL);
    lock->readers = 0;
    lock->q = cola_crear();
    return lock;
}

void rwlock_read_lock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lock);

    _lock(lock);

    if (lock->readers == 0)
        pthread_mutex_lock(&lock->mLock); // inmediato porque no puede haber escritores (sino no sería el primero de la cola)
    
    lock->readers++;
    lock->q = cola_desencolar(lock->q, (FuncionDestructora)nodeDestroy);
    pthread_mutex_unlock(&lock->lock);
}

void rwlock_read_unlock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lock);

    lock->readers--;

    if (lock->readers == 0)
        pthread_mutex_unlock(&lock->mLock);

    _unlock(lock);

    pthread_mutex_unlock(&lock->lock);
}

void rwlock_write_lock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lock);

    _lock(lock);

    pthread_mutex_unlock(&lock->lock);

    pthread_mutex_lock(&lock->mLock); // Se puede quedar esperando a que terminen readers
}

void rwlock_write_unlock(rwlock_t* lock) {
    pthread_mutex_lock(&lock->lock);

    lock->q = cola_desencolar(lock->q, (FuncionDestructora)nodeDestroy);

    _unlock(lock);

    pthread_mutex_unlock(&lock->mLock);
    pthread_mutex_unlock(&lock->lock);
}

void rwlock_destroy(rwlock_t* lock) {
    pthread_mutex_destroy(&lock->lock);
    pthread_mutex_destroy(&lock->mLock);
    cola_destruir(lock->q, (FuncionDestructora)nodeDestroy);
    free(lock);
}