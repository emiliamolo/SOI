#include <stdio.h>
#include "mutex_recursivo.h"

mutex_recursivo_t mr;
int i= 0;
void funcion_interna() {
    mutex_recursivo_lock(&mr);
    printf("En funcion_interna %d\n", i);
    if (i < 10) {
        i++;
        funcion_interna();
    }
    
    mutex_recursivo_unlock(&mr);
}

void* funcion_externa(void* _) {
    mutex_recursivo_lock(&mr);
    printf("En funcion_externa\n");
    funcion_interna();
    mutex_recursivo_unlock(&mr);
    return NULL;
}

int main() {
    mutex_recursivo_init(&mr);
    pthread_t externa;
    pthread_create(&externa, NULL, funcion_externa, NULL);

    pthread_join(externa, NULL);
    mutex_recursivo_destroy(&mr);
    return 0;
}