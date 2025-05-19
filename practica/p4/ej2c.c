#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include "timing.h"

#define N 5*100000000
#define N_THR 10 // división exacta

pthread_mutex_t mutex;
double sum = 0;

double a[N];
void* suma(void* param) {
    int id = param - (void*)0;
    int n = N / N_THR;
    int l = id*n, r = (id+1)*n;
    for (int i = l; i < r; i++) {
        pthread_mutex_lock(&mutex);
        sum += a[i];
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void init() {
    pthread_t thr[N_THR];
    for (int i=0; i < N_THR; i++)
        pthread_create(&thr[i], NULL, suma, i + (void*)0);

    for (int i=0; i < N_THR; i++)
        pthread_join(thr[i], NULL);
}

int main(){
    int val = 1;

    for (int i=0; i < N; i++){
        a[i] = (rand()%1000)/100.0;
    }

    pthread_mutex_init(&mutex, NULL);

    float tim;
    TIME_void(init(), &tim);

    printf("sum: %f, tardó %f\n", sum, tim);

    return 0;
}

/*
Tiempo (wall):
- a): 0.23429s
- b): 1.10186s
- c): 38.76216s

Conclusión: OpenMP es más rápido, secuencial le sigue, y con mutex CLARAMENTE es más lento
*/