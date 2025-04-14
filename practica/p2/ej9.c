#include <stdio.h>

#define NUM_VISITANTES 200000

int visitantes = 0;

int main() {
    for (int i = 0; i < NUM_VISITANTES; i++)
        visitantes++;
    
    printf("Visitantes: %d\n",visitantes);

    return 0;
}

// Sin sincronización o usando un sólo thread tarda alrededor de 0.010s, mientras que con pthread_mutex
// o incl (usando con y sin lock) tarda 0.02 - 0.03s. Es decir, el costo de implementar concurrencia resolviendo
// condiciones de carrera es mayor al implementar el problema del jardín ornamental en un sólo hilo.
// Además, el uso de incl con lock es 0.005s más lento que sin, mostrando un costo adicional en la atomicidad de incl
// (aunque no lo sea respecto a multicore).