#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "timing.h"


int esprimo(long n) {
    int cant = 0;
    long raiz = sqrtl(n);
    #pragma omp parallel for reduction(+: cant) 
    for(long i=2;i<=raiz;i++){
        if (n % i == 0) cant++;
    }
    return cant == 0;
}
    
int main(){

    float tim;
    int primo = TIME(esprimo(1000000000000000003*2), &tim);
   
    printf("primo: %d, tardó %f\n", primo, tim);

    return 0;
}
