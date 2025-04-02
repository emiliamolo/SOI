#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (!pid) {
        printf("CHILD %d\n",getpid());
        exit(0);
    }
    else {
        printf("PARENT %d\n",getpid());
        while (1) {}
    }

    /* El hijo aparece en la tabla de procesos como [] <defunct>.
    Esto se debe a que cuando un proceso hijo termina sin que el padre lo espere,
    el proceso hijo queda en estado zombie. De hecho, en la tabla de procesos, aparece también
    como "Z+", indicando "Z" que se encuentra, como zombie, mientras el padre está "R+", "R" de 
    running:
        20619 pts/0    R+     0:03 ./fork_inf
        20620 pts/0    Z+     0:00 [fork_inf] <defunct>
    Ésto está aclarado en la sección NOTES de `man 2 wait`:

        A child that terminates, but has not been waited for  becomes  a  "zom‐
       bie".  The kernel maintains a minimal set of information about the zom‐
       bie process (PID, termination status, resource  usage  information)  in
       order to allow the parent to later perform a wait to obtain information
       about the child.  As long as a zombie is not removed  from  the  system
       via  a wait, it will consume a slot in the kernel process table, and if
       this table fills, it will not be possible to create further  processes.
       If a parent process terminates, then its "zombie" children (if any) are
       adopted by init(1), (or by the nearest "subreaper" process  as  defined
       through  the  use  of  the  prctl(2) PR_SET_CHILD_SUBREAPER operation);
       init(1) automatically performs a wait to remove the zombies.

    El motivo de la existencia de los procesos zombie es porque el padre puede requerir
    la información del hijo cuando terminó, como su pid, el estado de terminación, etc.
    El hijo no puede saber cuándo el padre hace su wait, si bien puede hacerlo después de
    haber terminado su ejecución.
    */

    return 0;
}