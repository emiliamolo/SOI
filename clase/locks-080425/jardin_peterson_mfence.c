#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_VISITANTES 20000

int visitas = 0;
int flag[2] = {0,0}; // i tiene intención de ejecutarse
int turno = 0; // 1 -> molinete 1, 2 -> molinete 2

void* molinete1(void *_) {
    int cnt = NUM_VISITANTES / 2;
    for (int i = 0; i < cnt; i++) {
        flag[0] = 1;
        turno = 2;
        asm("mfence");
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
        turno = 1;
        asm("mfence");
        while(flag[0] && turno == 1){}
        visitas++;
        //printf("[M2] Visita %d\n",visitas);
        flag[1] = 0;
    }
    return NULL;
}

// OJO: Se rompe en multicore por ejecución fuera de orden
// y store buffering.

int main() {
    pthread_t molinete_1, molinete_2;
    pthread_create(&molinete_1, NULL, molinete1, NULL);
    pthread_create(&molinete_2, NULL, molinete2, NULL);

    pthread_join(molinete_1, NULL);
    pthread_join(molinete_2, NULL);

    printf("Visitantes: %d\n",visitas);

    return 0;
}