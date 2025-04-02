#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int arr[1000000];

    for (int i = 0; i < 1000000; i++)
        arr[i] = 333;

    pid_t pid = fork();

    if (!pid) {
        puts("HIJOOOOOO");
        for (int i = 0; i < 1000000; i++)
            arr[i] = 3745;
        while(1){}
    }
    else {
        puts("PADRE");
        for (int i = 0; i < 1000000; i++)
            arr[i] = 44545;
        while(1){}
    } // se duplica la memoria, pero sólo se copia físicamente a medida que se necesita (copy-on-write)
    

    /*
    sleep(15);

    execl("/bin/bash","/bin/bash",NULL); // Se reemplaza la memoria del proceso original por la del ejecutado por exec.
    */

    return 0;
}