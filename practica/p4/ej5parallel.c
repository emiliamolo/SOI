#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timing.h"

#define N 1000
int A[N][N], B[N][N], C[N][N];

void mult(int A[N][N], int B[N][N], int C[N][N])
{
    int i, j, k;
    #pragma omp parallel for  
        for (i = 0; i < N; i++)
        #pragma omp parallel for 
            for (k = 0; k < N; k++)
            #pragma omp parallel for  
            for (j = 0; j < N; j++)             
                C[i][j] += A[i][k] * B[k][j];
}

void mult_tras(int A[N][N], int B[N][N], int C[N][N])
{
    int i, j, k;
    #pragma omp parallel for  
     for (j = 0; j < N; j++)    
        #pragma omp parallel for 
        for (i = 0; i < N; i++)
            #pragma omp parallel for  
             for (k = 0; k < N; k++)     
                C[i][j] += A[i][k] * B[j][k];
}

int main()
{
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = random() % 1000;
            B[i][j] = random() % 1000;
        }
    }

    float tim;
    TIME_void(mult_tras(A,B,C), &tim);
   

    return 0;
}

/*
Observamos una reducción del tiempo de un 22% utilizando OpenMP para 1000x1000, 20% para 500x500, y 26% para 200x200.
*/

/*
b)
Probando todas las formas de cambiar los índices comprobamos que se puede mejorar el rendimiento siempre y cuando se
use el índice j en el úiltimo for. La performance según si está k primero o segundo es muy similar.
Hipotetizamos que esto se debe a que tanto el arreglo C como el arreglo B se acceden de forma secuencial en una misma fila,
aprovechando mejor las lecturas y escrituras entre la caché y la memoria, ya que al escribir/leer de memoria, primero
se lo hace en la caché, y luego en la memoria en sí. De esta forma, al modificar varias posiciones consecutivas en memoria
de forma secuencial logramos hacer estas operaciones directamente sobre/desde la caché, disminuyendo el tiempo de ejecución
reduciendo los accesos a memoria.
*/

/*
c) 
Para multiplicar A * B^t, el orden optimo seria el original, i j k.
Igual que antes, se aprovechan mejor las lecturas/escrituras entre la cache y la memoria. Pero como se multiplica A y B^t
Esto es porque lo que antes se optimizaba accediendo de forma secuencial por filas a A y C por filas, ahora se aprovecha accediendo
a B y C por filas. Esta es la mejor forma de acceder a los arreglos y hacer la multiplicación porque dos de 3 tienen como último índice
k. 
*/