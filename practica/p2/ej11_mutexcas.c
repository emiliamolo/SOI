#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_VISITANTES 100000

int visitas = 0;
int mut = 0;

int CAS(int *l, int a, int b) {
    int r;
    asm("movq $0, %%rax\n"
        "movq $1, %%rbx\n"
        "lock cmpxchg %%rbx, (%1)"
        : "=@ccz"(r) : "r"(l) : "rax", "rbx", "memory");
    return r;
}

void lock(int *l) {
    while(!CAS(l, 0, 1));
}

void unlock(int *l) {
    asm("movq $0, (%0)" : : "r"(l) : "memory");
}

void* molinete(void *_) {
    int cnt = NUM_VISITANTES;
    for (int i = 0; i < cnt; i++) {
        lock(&mut);
        visitas++;
        unlock(&mut);
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
