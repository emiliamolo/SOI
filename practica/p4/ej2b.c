#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timing.h"

#define N 5*100000000
double a[N];    

double sumf() {
    double sum = 0;
    for(int i=0;i<N;i++){
        sum+=a[i];
    }
    return sum;
}

int main(){
    int val = 1;

    for (int i=0; i < N; i++){
        a[i] = (rand()%1000)/100.0;
    }

    // secuencial
    float tim;
    double sum = TIME(sumf(), &tim);
   
    printf("sum: %f, tardó %f\n", sum, tim);

    return 0;
}