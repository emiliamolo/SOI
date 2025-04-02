#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int denom = 0; 
void handler(int s) { 
    printf("ouch!\n"); 
    denom = 1; 
    //exit(1);
} 

int main() { 
    int r; 
    signal(SIGFPE, handler); 
    r = 1 / denom; 
    printf("r = %d\n", r);

    //printf("denom %d\n", denom); 
    return 0; 
}

/*
8) Que pasa al correr el programa? Por que?

Cuando se genera la señal SIGFPE (error de coma flotante, en este caso por división entre cero),
el manejador (handler) se ejecuta y cambia denom a 1. 
Sin embargo, el programa no vuelve a ejecutar la división con el nuevo valor de denom, 
sino que simplemente reintenta la operación fallida.

El comportamiento del sistema operativo no garantiza que la operación se reintente
con el nuevo valor de denom.???
La ejecución principal del programa no necesariamente "recoge" los cambios hechos en el 
manejador porque ya estaba en un estado inconsistente debido a la excepción.



- Agregando exit(1) no se logra cambiar el denom, simplemente termina el programa
- Se puede hacer que si cambie y siga con longjmp/siglongjmp pero de todas formas el programa
queda en un estado indeterminado o inseguro. En general, no sabemos como se genero SIGFPE, aunque en este cado medio que si
y continuar con la ejecucion podría generar mas daño, no tendría mucho sentido intentar continuar la ejecucion.

https://stackoverflow.com/questions/39431879/c-handle-signal-sigfpe-and-continue-execution


*/
