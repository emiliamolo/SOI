#include <stdio.h>
#include <unistd.h>
int main(){
    while(1){
        char buf[20];
        scanf("%s", buf);
        printf("%s\n", buf);
        sleep(3);
    }

    // Sin pipeline consume mucha CPU (casi toda) porque se está consumiendo la salida constantemente
    // Con un pipe, en cambio, se llena el buffer del pipe y se bloquea printf hasta que se consuma del buffer,
    // entonces se ejecuta tras el delay establecido, consumiendo prácticamente nada de CPU.

    // https://stackoverflow.com/questions/70254889/pipes-in-c-buffer-for-reading-stdin

    return 0;
}