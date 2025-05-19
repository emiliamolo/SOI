#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timing.h"

#define N 5*100000000

double a[N];

double minimo() {
    double min = a[0];
    #pragma omp parallel for reduction(min: min) 
    for(int i=0;i<N;i++){
        if (a[i] < min) min = a[i];
    }
    return min;
}
    
int main(){
    int val = 1;

    for (int i=0; i < N; i++){
        a[i] = 100.387;
    }
    a[92483] = 19.846;

    float tim;
    double min = TIME(minimo(), &tim);
   
    printf("min: %f, tardó %f\n", min, tim);

    return 0;
}

/*
Claramente
- de forma paralela tarda: 0.23130s
- de forma secuencial: 0.9813s

*/