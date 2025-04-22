#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_VISITANTES 20000000

int visitas = 0;
int flag[2] = {0,0}; // i tiene intención de ejecutarse
int turno = 0; // 1 -> molinete 1, 2 -> molinete 2

void* molinete1(void *_) {
    int cnt = NUM_VISITANTES / 2;
    for (int i = 0; i < cnt; i++) {
        flag[0] = 1;
        asm("mfence");
        turno = 2;
        while(flag[1] && turno == 2){}
        visitas++;
        //printf("[M1] Visita %d\n",visitas);
        flag[0] = 0;
    }
    return NULL;
}
void* molinete2(void *_) {
    int cnt = NUM_VISITANTES / 2;
    for (int i = 0; i < cnt; i++) {
        flag[1] = 1;
        asm("mfence");
        turno = 1;
        
        while(flag[0] && turno == 1){}
        visitas++;
        //printf("[M2] Visita %d\n",visitas);
        flag[1] = 0;
    }
    return NULL;
}



int main() {
    pthread_t molinete_1, molinete_2;
    pthread_create(&molinete_1, NULL, molinete1, NULL);
    pthread_create(&molinete_2, NULL, molinete2, NULL);

    pthread_join(molinete_1, NULL);
    pthread_join(molinete_2, NULL);

    printf("Visitantes: %d\n",visitas);

    return 0;
}
/*
El mfence garantiza que todas las instrucciones anteriores sean visibles antes que las posteriores al mfence.
Pero si está antes que la asignación del turno, no se asegura el orden necesario entre la asignación de la bandera
y del turno. 
Por ejemplo: si  molinete1 ejecuta flag[0] = 1, molinete2 puede leer turno=2 y entrar a la 
región crítica pensando que tiene prioridad. A la vez, molinete1 puede, de la misma forma, leer turno=1 y también entrar 
a la región crítica. 
*/