
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#define N 10000
int visitantes = 0;

void * proc(void *arg) {
    int i;
    int id = arg - (void*)0;
    for (i = 0; i < N; i++) {
        int c;
        c = visitantes;
        sleep(.00001);
        visitantes = c + 1;
    }

    return NULL;
}

int main() {
    pthread_t molinete_1, molinete_2;
    pthread_create(&molinete_1, NULL, proc, NULL);
    pthread_create(&molinete_2, NULL, proc, NULL);

    pthread_join(molinete_1, NULL);
    pthread_join(molinete_2, NULL);

    printf("Visitantes: %d\n",visitantes);

    return 0;
}