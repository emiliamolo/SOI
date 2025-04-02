#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    sleep(10);
    int* a = malloc(1024*1024*1024);

    for (int i = 0; i < 1020004; i++)
        a[i] = 3333;
    printf("OK\n");
    while(1);

    // No aumenta la memoria del proceso pero si se reserva la memoria virtual, por lo que la memoria virtual si aumenta
    // Si se ocupa un poco mas de memoria realizar la reserva en tabla de paginacion del proceso pero no es mucho y no es parte de la memoria del proceso
    // Si se ocupa la memoria, como en esta caso, entonces si aumenta la memoria al asignarle una direccion fisica a esas direcciones virtuales que se habian reservado.
    // Estos cambios los vimos usando top (htop) comparando el valor de VIRT y RES (memoria virtual y física utilizadas del proceso).

    return 0;
}