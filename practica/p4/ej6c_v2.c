#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timing.h"

#define M 1000000

void swap(int* a, int* b){
    int c = *a;
    *a = *b;
    *b = c;
}
/* Particion de Lomuto, tomando el primer elemento como pivote */
int particionar(int a[], int N)
{
    int i, j = 0;
    int p = a[0];
    swap(&a[0], &a[N-1]);
    for (i = 0; i < N-1; i++) {
        if (a[i] <= p)
            swap(&a[i], &a[j++]);
    }
    swap(&a[j], &a[N-1]);
    return j;
}
void miqsort(int a[], int N)
{
    if (N < 2)
        return;
    int p = particionar(a, N);
    #pragma omp task if (p > 2000)
    {
        miqsort(a, p);
    }
    int p2 = N-p-1;
    #pragma omp task if (p2 > 2000)
    {
        miqsort(a + p + 1, p2);
    }
}

void run(int a[], int N) {
    #pragma omp parallel
    #pragma omp single
    #pragma omp taskgroup
    {
        #pragma omp task
        miqsort(a, N);
    }
}

int main(){
    int a[M];
    for(int i=0; i<M;i++){
        a[i] = rand()%1000;
    }

    float tim;
    TIME_void(run(a,M), &tim);
    // for(int i=0; i <M; i++){
    //     printf("%d  ", a[i]);
    // }
    return 0;
}

/*
Funciona bastante más rápido. Con N = 1e6:
Secuencial: 2.01875s
OpenMP: 1.62609s
OpenMP Tasks: 0.51934s (0.43517s otra PC)
OpenMP Tasks (v2): 0.11229s (otra PC)

Como una optimización adicional, se puede usar la cláusula if para evitar crear threads en particionar fragmentos
pequeños del array, donde el costo de crear tasks es más costoso que lo ahorrado en paralelizar.
Observamos así respecto a sin esta optimización, un speedup de alrededor de 3.87454 aunque una eficiencia de 0.32295
(12 posibles threads).
*/
