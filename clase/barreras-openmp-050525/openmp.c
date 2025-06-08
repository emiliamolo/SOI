#include <stdio.h>
#include <omp.h>

int main(){
    // lanza tantos threads como procesadores
    // lo maneja solo si no e intenta paralelizar al maximo, se puede limitar
    // todos hacen lo mismo
    #pragma omp parallel 
    {
        int id = omp_get_thread_num();
        int nt = omp_get_num_threads();

        printf("Thread id: %d\n", id);
        printf("Numero de threads: %d\n", nt);
        printf("Hola\n");
    }
    return 0;
}
// compilar con -fopenmp
// si no, lo hace de forma secuencial, ignora la parte de pragma 