#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "timing.h"
#define M 500000000
// Función para combinar dos mitades ordenadas del array
void merge(int arr[], int izquierda, int medio, int derecha) {
    int i, j, k;
    int n1 = medio - izquierda + 1;
    int n2 = derecha - medio;

    // Crear arreglos temporales
    int L[n1], R[n2];

    // Copiar datos a arreglos temporales L[] y R[]
    #pragma omp parallel
    {
        #pragma omp parallel for
        for (i = 0; i < n1; i++)
            L[i] = arr[izquierda + i];
        #pragma omp parallel for
        for (j = 0; j < n2; j++)
            R[j] = arr[medio + 1 + j];
    }
    

    // Combinar los arreglos temporales de vuelta en arr[]
    i = 0; // Índice inicial del primer subarreglo
    j = 0; // Índice inicial del segundo subarreglo
    k = izquierda; // Índice inicial del subarreglo combinado
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[], si hay alguno
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si hay alguno
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Función principal de Merge Sort
void mergeSort(int arr[], int izquierda, int derecha) {
    if (izquierda < derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;

        // Ordenar la primera y la segunda mitad
        #pragma omp task
        mergeSort(arr, izquierda, medio);
        #pragma omp task
        mergeSort(arr, medio + 1, derecha);

        #pragma omp taskwait
        // Combinar las mitades ordenadas
        merge(arr, izquierda, medio, derecha);
    
    }
}

// Función para imprimir el arreglo
void imprimirArreglo(int arr[], int tamano) {
    for (int i = 0; i < tamano; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void run(int a[], int N){
    #pragma omp parallel
    #pragma omp single
    #pragma omp taskgroup
    #pragma omp task
    mergeSort(a, 0, N - 1);
}

// Ejemplo de uso
int main() {
    int a[M];
    for(int i=0; i<M;i++){
        a[i] = rand()%1000;
    }
    float tim;
    TIME_void(run(a,M), &tim);

    /*
    for (int i = 0; i < M; i++)
        printf("%d ",a[i]);
    */

    return 0;
}
