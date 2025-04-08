#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_VISITANTES 2000000

int visitas = 0;
int flag[2] = {0,0}; // i tiene intención de ejecutarse

void* molinete1(void *_) {
    int cnt = NUM_VISITANTES / 2;
    for (int i = 0; i < cnt; i++) {
        flag[0] = 1;
        while(flag[1]){}
        visitas++;
        printf("[M1] Visita %d\n",visitas);
        flag[0] = 0;
    }
    return NULL;
}
void* molinete2(void *_) {
    int cnt = NUM_VISITANTES / 2;
    for (int i = 0; i < cnt; i++) {
        flag[1] = 1;
        while(flag[0]){}
        visitas++;
        printf("[M2] Visita %d\n",visitas);
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