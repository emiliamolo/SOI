#include "mutex_recursivo.h"


int mutex_recursivo_init(mutex_recursivo_t *mr){
    mr->cantidad = 0;
    pthread_mutex_init(&mr->mutex, NULL);
    mr->propietario = gettid();
}

int mutex_recursivo_lock(mutex_recursivo_t *mr){
    int pid = gettid();
    if (pid == mr->propietario){
        mr->cantidad++;
        return 0;
    }
    int res = pthread_mutex_lock(&mr->mutex);
    if (res == 0){
        mr->propietario = pid;
        mr->cantidad = 1;
    }
    return res;
}

int mutex_recursivo_unlock(mutex_recursivo_t *mr){
    if (mr->propietario != gettid()){
        return -1;
    }
    mr->cantidad--;
    if(mr->cantidad == 0){
        mr->propietario = 0;
        return pthread_mutex_unlock(&mr->mutex);
    }
    return 0;
}

int mutex_recursivo_destroy(mutex_recursivo_t *mr){
    return pthread_mutex_destroy(&mr->mutex);
}