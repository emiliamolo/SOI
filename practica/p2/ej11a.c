#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_VISITANTES 100000

int visitas = 0;

void* molinete(void *_) {
    int cnt = NUM_VISITANTES;
    for (int i = 0; i < cnt; i++) {
        asm("movq %0, %%rax\n"
            "while:\n"
                "movq $1, %%rbx\n"
                "addq %%rax, %%rbx\n"
                "cmpxchg %%rbx, %0\n" // Funciona sólo con un solo núcleo (pero varios threads)
                "jnz while"
            : "+m"(visitas) : : "rax", "rbx", "memory");
    }
    return NULL;
}

int main() {
    pthread_t molinetes[100];
    for (int i = 0; i < 100; i++)
        pthread_create(&molinetes[i], NULL, molinete, NULL);

    for (int i = 0; i < 100; i++)
        pthread_join(molinetes[i], NULL);

    printf("Visitantes: %d\n",visitas);

    return 0;
}