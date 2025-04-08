#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define MAX 1000000

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER, mutex2 = PTHREAD_MUTEX_INITIALIZER;
int r1 = 0, r2 = 0;

void* a(void *_) {
    for (int i = 0; i < MAX; i++) {
        puts("[a] New cycle. Taking mutex1... ");
        pthread_mutex_lock(&mutex1); // ORDEN! -> DEADLOCK
        puts("[a] Taken mutex1. Taking mutex2... ");
        pthread_mutex_lock(&mutex2);
        puts("[a] Taken mutex2");
        r1 = r2 + 1;
        puts("[a] Unlocking mutex1");
        pthread_mutex_unlock(&mutex1);
        puts("[a] Unlocked mutex1. Unlocking mutex2...");
        pthread_mutex_unlock(&mutex2);
        puts("[a] Unlocked mutex2");
    }
}

void* b(void *_) {
    for (int i = 0; i < MAX; i++) {
        puts("[b] New cycle. Taking mutex2... ");
        pthread_mutex_lock(&mutex2); // ACÁ EL ORDEN IMPORTA, AL REVÉS DE EN a() -> DEADLOCK!
        puts("[b] Taken mutex2. Taking mutex1... ");
        pthread_mutex_lock(&mutex1);
        puts("[b] Taken mutex1");
        r2 = r1 + 1;
        puts("[b] Unlocking mutex2");
        pthread_mutex_unlock(&mutex2); // Se pueden cambiar estos de lugar e igual hay deadlock
        puts("[b] Unlocked mutex2( Unlocking mutex1...");
        pthread_mutex_unlock(&mutex1);
        puts("[b] Unlocked mutex1");
    }
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, a, NULL);
    pthread_create(&thread2, NULL, b, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);;

    return 0;
}