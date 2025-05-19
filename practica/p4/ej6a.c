#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timing.h"
#define M 100000

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
void* miqsort(void** aux)
{
    int* a = aux[0];
    int N = aux[1] - (void*) 0;
    if (N < 2)
        return NULL;
    int p = particionar(a, N);
    pthread_t thread1, thread2;

    typedef void* (*hola)(void*);
    void* aux1[2] = {a,p+(void*)0};
    pthread_create(&thread1, NULL, (hola)miqsort, aux1);
    void* aux2[2] ={a + p + 1, N - p - 1 + (void*)0};
    pthread_create(&thread2, NULL, (hola)miqsort, aux2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return NULL;
}

int main(){
    int a[M];
    for(int i=0; i<M;i++){
        a[i] = rand()%1000;
    }

    float tim;
    void* aux[2] = {a,M+(void*)0};
    TIME_void(miqsort(aux), &tim);
    // for(int i=0; i <M; i++){
    //     printf("%d  ", a[i]);
    // }
    return 0;
}

/*
Si, hay un problema: con un arreglo muy grande se crean demasiados threads, al punto que se excede el límite del stack, causando
un segmentation fault. En efecto, dado un arreglo de N elementos, se hacen al menos N, que con por ejemplo, N = 100000, es demasiado.
*/