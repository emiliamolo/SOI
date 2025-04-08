#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_VISITANTES 200000

int visitas = 0;
int flag = 0;//1 locked

void* molinete(void *_) {
    int cnt = NUM_VISITANTES / 2;
    for (int i = 0; i < cnt; i++) {
        while(flag == 1){;}
        flag = 1; // lock
        visitas++;
        flag = 0; // unlock
    }
    return NULL;
}

int main() {
    pthread_t molinete1, molinete2;
    pthread_create(&molinete1, NULL, molinete, NULL);
    pthread_create(&molinete2, NULL, molinete, NULL);

    pthread_join(molinete1, NULL);
    pthread_join(molinete2, NULL);

    printf("Visitantes: %d\n",visitas);

    return 0;
}