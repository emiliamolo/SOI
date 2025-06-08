#include <stdio.h>
#include <omp.h>

int main(){
    int sum = 0;
    int val = 1;
    // paraleliza el for, y cada uno tiene una copia de sum, 
    // suma internamente y al final se suman todos los subresultados
    // sin reduction tendriamos una reace condition, hace algo como un private o algo interno?
    #pragma omp parallel
    {        
        for(int i=0;i<100000;i++) {
            #pragma omp critical
                sum++;
        }
        
    }
    printf("sum:%d\n", sum);
    // hay una barrera implicita, espera a q todos los threads terminen para seguir acá.
    return 0;
}
// compilar con -fopenmp
// si no, lo hace de forma secuencial, ignora la parte de pragma 