#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <signal.h>

/*
 * Para probar, usar netcat. Ej:
 *
 *      $ nc localhost 4040
 *      NUEVO
 *      0
 *      NUEVO
 *      1
 *      CHAU
 */


#define SHM_NAME "/mi_memoria_compartida"
/* la memoria compartida creada con shm_open se representa como un archivo
 especial en el directorio /dev/shm/*/

void quit(char *s)
{
    perror(s);
    abort();
}

int fd_readline(int fd, char *buf)
{
    int rc;
    int i = 0;

    /*
     * Leemos de a un caracter (no muy eficiente...) hasta
     * completar una línea.
     */
    while ((rc = read(fd, buf + i, 1)) > 0) {
        if (buf[i] == '\n')
            break;
        i++;
    }

    if (rc < 0)
        return rc;

    buf[i] = 0;
    return i;
}

void handle_conn(int csock, int *shared_u)
{
    char buf[200];
    int rc;

    while (1) {
        /* Atendemos pedidos, uno por linea */
        rc = fd_readline(csock, buf);
        if (rc < 0)
            quit("read... raro");

        if (rc == 0) {
            /* linea vacia, se cerró la conexión */
            close(csock);
            return;
        }

        if (!strcmp(buf, "NUEVO")) {
            char reply[20];
            sprintf(reply, "%d\n", *shared_u);
            (*shared_u)++;
            write(csock, reply, strlen(reply));
        } else if (!strcmp(buf, "CHAU")) {
            close(csock);
            return;
        }
    }
}
void wait_for_clients(int lsock, int *shared_u)
{
    int csock;
    /* Esperamos una conexión, no nos interesa de donde viene */
    csock = accept(lsock, NULL, NULL);
    if (csock < 0)
        quit("accept");

    int pid = fork();
    if (pid == 0){
        /* Atendemos al cliente */
        handle_conn(csock, shared_u);
        munmap(shared_u, sizeof(int));
        exit(0);
    }
    else{
        /* Volvemos a esperar conexiones */
        wait_for_clients(lsock, shared_u);
    }
}

/* Crea un socket de escucha en puerto 4040 TCP */
int mk_lsock()
{
    struct sockaddr_in sa;
    int lsock;
    int rc;
    int yes = 1;

    /* Crear socket */
    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0)
        quit("socket");

    /* Setear opción reuseaddr... normalmente no es necesario */
    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
        quit("setsockopt");

    sa.sin_family = AF_INET;
    sa.sin_port = htons(4040);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bindear al puerto 4040 TCP, en todas las direcciones disponibles */
    rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
    if (rc < 0)
        quit("bind");

    /* Setear en modo escucha */
    rc = listen(lsock, 10);
    if (rc < 0)
        quit("listen");

    return lsock;
}

void sigint_handler() {
    int err = shm_unlink(SHM_NAME);
    exit(0);
}

int main()
{
    // Podríamos hacer shm_uinlink al comienzo del programa para limpiar, pero lo ideal es que se limpie cuando termine.
    // Usamos signal suponiendo que la forma "correcta" de terminar el servidor es usando Ctrl+C
    signal(SIGINT, sigint_handler);

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // Crear o abrir la memoria compartida
    ftruncate(fd, sizeof(int)); // El objeto de memoria compartida inicialmente tiene tamaño 0 (man shm_open). Debemos aumentarlo al tamaño necesario.
    int *shared_u = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (shared_u == MAP_FAILED) {
        perror("Error en mmap");
        exit(1);
    }
    *shared_u = 0; // Tiraría Bus error sin ftruncate
    
    int lsock;
    lsock = mk_lsock();
    wait_for_clients(lsock, shared_u);
}
