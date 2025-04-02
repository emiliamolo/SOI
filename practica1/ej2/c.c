#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int d = dup(1);
    write(d, "Hola\n", 6);
    
    close(d);
    write(1, "hola\n", 6);
    // no se cierra 
    return 0;
}