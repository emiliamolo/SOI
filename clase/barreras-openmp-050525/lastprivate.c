#include <stdio.h>
#include <omp.h>

int main(){
    int a[100];
    int i;
    #pragma omp parallel 
    {
        #pragma omp for lastprivate(i)
        for (i = 0; i < 100; i++){ // lo divide en varios threads, en cantidades iguales, todos acceden al array compartido
            a[i] = 2 * i;
            printf("a[%d]: %d - id: %d - num_threads:%d\n",i, a[i], omp_get_thread_num(), omp_get_num_threads());
        }
    }
    printf("i: %d\n", i);

    // hay una barrera implicita, espera a q todos los threads terminen para seguir acá.
    return 0;
}
// compilar con -fopenmp
// si no, lo hace de forma secuencial, ignora la parte de pragma 