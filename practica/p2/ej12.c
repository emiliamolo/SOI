#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;

void * foo(void *_arg)
{
    pthread_mutex_lock(&a);
    printf("Foo!\n");
    sleep(1);
    pthread_mutex_unlock(&a);
    return NULL;
}

int main()
{
    pthread_t th;
    pthread_create(&th, NULL, foo, NULL);
    pthread_mutex_t b = a;
    pthread_mutex_lock(&b);
    printf("Bar!\n");
    sleep(1);
    pthread_mutex_unlock(&b);
    pthread_join(th, NULL);
    return 0;
}

// Dado que b se inicializa casi inmediatamente después de crear el thread, se initializa b con PTHREAD_MUTEX_INITIALIZER,
// resultando que ambos mutex son distintos, y por ende funcionan por separado. Debido a esto, ambos print se hacen casi
// en simultáneo.
// Si se pusiera un sleep suficientemente corto antes de la inicialización de b tal que a se lockee, entonces se copia
// a b un mutex lockeado, produciéndose un deadlock porque b espera a que se desbloquee, cuando son mutex distintos.
// Si el sleep es más largo, se puede llegar a copiar desbloqueado sin inconvenientes (sleep(1) vs sleep(3)).