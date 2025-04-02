#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    close(1);

    char buf[255];
    strcpy(buf,"HOLA\n");
    int a = write(1,buf,strlen(buf));
    fprintf(stderr,"returned %d\n",a);

    // Al cerrar el fd de la salida estándar, escribir en ella resulta en un error.
    // Con write, por ejemplo, devuelve -1.
    
    return 0;
}