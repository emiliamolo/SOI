#include <stdio.h>
#include <omp.h>

int main(){
    int num =  2; // num_thread incluye al master, se puede extender, no ayuda mucho
    #pragma omp parallel sections num_threads(num)
    {
        printf("Antes\n"); // es una seccion implicita (?, se ejecuta una sola vez
        #pragma omp section //cada seccion corre por un solo thread
        {
            int id = omp_get_thread_num();
            printf("1) section 1, thread %d\n", id);
            int nt = omp_get_num_threads();

            printf("1)Thread id: %d\n", id);
            printf("1)Numero de threads: %d\n", nt);
        }
        #pragma omp section
        {
            printf("section 2\n");
            int id = omp_get_thread_num();
            int nt = omp_get_num_threads();

            printf("2)Thread id: %d\n", id);
            printf("2)Numero de threads: %d\n", nt);
        }
    }

    // hay una barrera implicita, espera a q todos los threads terminen para seguir acá.
    return 0;
}
// compilar con -fopenmp
// si no, lo hace de forma secuencial, ignora la parte de pragma 