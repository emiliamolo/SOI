#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_VISITANTES 200000

int visitas = 0;

void* molinete(void *_) {
    int cnt = NUM_VISITANTES / 2;
    for (int i = 0; i < cnt; i++) {
        visitas++;
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

/*
Cuando compilamos con optimizaciones, directamente se suma NUM_VISITANTES a visitas, no se ejecuta el for.
    molinete:
    .LFB36:
        .cfi_startproc
        endbr64
        addl	$100000, visitas(%rip)
        xorl	%eax, %eax
        ret
        .cfi_endproc
*/