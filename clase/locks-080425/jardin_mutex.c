#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_VISITANTES 20000

int visitas = 0;
pthread_mutex_t mutex_visitantes = PTHREAD_MUTEX_INITIALIZER;

void* molinete(void *_) {
    int cnt = NUM_VISITANTES / 2;
    for (int i = 0; i < cnt; i++) {
        // lock
        pthread_mutex_lock(&mutex_visitantes);
        visitas++;
        // unlock
        pthread_mutex_unlock(&mutex_visitantes);
    }
    return NULL;
}

int main() {
    pthread_t molinete_1, molinete_2;
    pthread_create(&molinete_1, NULL, molinete, NULL);
    pthread_create(&molinete_2, NULL, molinete, NULL);

    pthread_join(molinete_1, NULL);
    pthread_join(molinete_2, NULL);

    printf("Visitantes: %d\n",visitas);

    return 0;
}