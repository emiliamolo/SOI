#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_VISITANTES 2000000
#define N 5

volatile int numeros[N];
volatile int eligiendo[N];
volatile int visitas = 0;

int max(int n, int numeros[n]){
    int res = numeros[0];
    for(int i=1; i < n; i++){
        if (numeros[i] > res) {
            res = numeros[i];
        }
    }
    return res;
}
void lock(int i){
    eligiendo[i] = 1;
    numeros[i] = 1 + max(N, numeros);
    eligiendo[i] = 0;
    for(int j=0;j < N; j++){
        while (eligiendo[j]) { ;/* busy waiting */ } /* Si el hilo j está calculando su número, espera a que termine */    
        
        /* Si el hilo j tiene más prioridad, espera a que ponga su número a cero */    
            /* j tiene más prioridad si su número de turno es más bajo que el de i */
            /* o bien si es el mismo número y además j es menor que i */
        // osea que dejo pasar a todos
        while((numeros[j] != 0) && ((numeros[j] < numeros[i]) || ((numeros[j] == numeros[i]) && (j < i)))); // busy waiting  
    }
}
void unlock(int i){
    numeros[i] = 0;
}
void* molinete(void* ii){
    int i = *(int*)ii;
    for (int k=0; k < NUM_VISITANTES; k++){
        lock(i);
        // seccion critica
        visitas++;
        // fin seccion critica
        unlock(i);
        
    }
    free(ii);
    return NULL;
}


int main(){
    for(int i=0;i< N;i++){
        eligiendo[i] = 0;
        numeros[i] = 0;
    }
    pthread_t hilos[N];
    for(int i=0;i< N;i++){
        void * arg = malloc(sizeof(int));
        *(int*)arg = i;
        pthread_create(&hilos[i], NULL, molinete, arg);
    }

    for (int i=0; i < N; i++){
        pthread_join(hilos[i], NULL);
    }

    printf("Visitantes: %d\n",visitas);
    return 0;
}

