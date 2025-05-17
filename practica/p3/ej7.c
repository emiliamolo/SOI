#include <pthread.h>
#include <stdio.h>

#define B 2
#define K (2*B)

volatile int s, r, buf[B];

static inline int diff() { return (K + s - r) % K; }
void * prod(void *_arg) {
    int cur = 0;
    while (1) {
        while (diff() == B)
                ;
        buf[s % B] = cur++;
        s = (s+1) % K;
    }
}
void * cons(void *_arg) {
    int cur;
    while (1) {
        while (diff() == 0)
                ;
        cur = buf[r % B];
        r = (r+1) % K;
        printf("Leí %d\n", cur);
    }
}

int main() {
    pthread_t consumer, producer;

    pthread_create(&consumer, NULL, cons, NULL);
    pthread_create(&producer, NULL, prod, NULL);

    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);
    
    return 0;
}

/*
Funciona sin mutexes ni primitivas de sincronización porque el productor y el 
consumidor modifican índices por separado, entonces en todo caso, uno de los dos
se queda esperando a ver si el otro produce algo nuevo / hay lugar para producir,
hasta que la caché del sistema se actualice. De esta forma, cada thread
independientemente tiene su valor actualizado mientras espera al otro a actualizarse
en la caché para continuar consumiendo / produciendo.
Esto también aplicaría si se extiende el tamaño de la queue, simplemente funciona
porque hay dos procesos con índices separados.
Por este motivo, no se puede generalizar a n procesos.
En efecto, si se agregara algún proceso como productor, pódría haber condiciones de
carrera entre ambos en el extremo de la cola que agrega elementos, pudiendo suceder
que alguno crea que hay lugar cuando el otro llenó la cola, o bien tener un índice
desactualizado y pisar elementos del otro productor. Algo similar sucedería si
se agrega, en cambio, un consumidor.
*/