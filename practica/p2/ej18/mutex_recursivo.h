#ifndef MUTEX_RECURSIVO_H
#define MUTEX_RECURSIVO_H

#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>

#define gettid() syscall(SYS_gettid)

typedef struct{
    pthread_mutex_t mutex;
    int cantidad;
    int propietario;
}mutex_recursivo_t;

int mutex_recursivo_init(mutex_recursivo_t *mr);
int mutex_recursivo_lock(mutex_recursivo_t *mr);
int mutex_recursivo_unlock(mutex_recursivo_t *mr);
int mutex_recursivo_destroy(mutex_recursivo_t *mr);

#endif // MUTEX_RECURSIVO_H