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
    #pragma omp task
    {
        miqsort(a, p);
    }
    #pragma omp task
    {
        miqsort(a + p + 1, N - p - 1);
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
OpenMP Tasks: 0.51934s
Es decir, un 70% más rápido que OpenMP con sections.
La diferencia entre usar sections y tasks es que los tasks no necesariamente esperan a que los individuales (cada "section") termine,
como si sucedía con sections. De esta forma, cada una de las llamadas se puede ejecutar de forma completamente independiente sin esperar
a los hijos del siguiente nivel, optimizando la ejecución.
*/