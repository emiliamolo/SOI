#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    close(0);

    char buf[255];
    int a = read(0,buf,50);
    printf("%d\n", a);

    // Al cerrar el fd de la entrada estándar, leer de ella resulta en un error.
    // Con read, por ejemplo, devuelve -1.
    
    return 0;
}