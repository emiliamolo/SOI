#include <stdio.h>
#include <omp.h>

int main(){
    int i = 10;
    #pragma omp parallel firstprivate(i) // va a ser privada de cada thread, pero se inicializa con el valor que traia
    {
        // i no esta inicializada
        printf("thread %d: i=%d\n", omp_get_thread_num(), i);
    }
    printf("thread %d: i=%d\n", omp_get_thread_num(), i);
    // hay una barrera implicita, espera a q todos los threads terminen para seguir acá.
    return 0;
}
// compilar con -fopenmp
// si no, lo hace de forma secuencial, ignora la parte de pragma 