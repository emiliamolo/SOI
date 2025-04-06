#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

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

#define MAX_BUF 255

#define MAX_VALUE 256
#define MAX_KEY 256
#define HASHMAP_SIZE 5003 // primo

#define NUM_CLIENTS 100

#define SHM_NAME "/shm_key_value_store" 
/* la memoria compartida creada con shm_open se representa como un archivo
 especial en el directorio /dev/shm/*/

typedef struct {
    char key[MAX_KEY]; // If empty, has \0 in first char
    char value[MAX_VALUE];
    int sig;
} CasillaHash;

typedef struct {
	CasillaHash hashmap[HASHMAP_SIZE];
    int used;
} Store;

int hash_str(char *s) {
    int hash = 0, p = 1, m = HASHMAP_SIZE;
    for (int i = 0; s[i]; i++) {
        hash = (hash + (s[i] * p) % m) % m;
        p = (p * 31) % m;
    }
    return hash;
}

void quit(char *s)
{
    perror(s);
    abort();
}

int fd_readline(int fd, char *buf, int n)
{
    int rc = recv(fd, buf, n, 0);
    if (rc < 0)
        return rc;
    
    int i;
    for (i = 0; buf[i] != '\n' && i < n; i++)
        if (!isalnum(buf[i]) && buf[i] != ' ')
            break;
    if (buf[i] != '\n')
        return -1;

    buf[i] = '\0';
    return rc;
}


void handle_conn(int csock, Store* store)
{
    char buf[MAX_BUF];
    int rc;

    while (1) {
        /* Atendemos pedidos, uno por linea */
        rc = fd_readline(csock, buf, MAX_BUF);
        if (rc < 0)
            quit("fd_readline");

        if (rc == 0) {
            /* linea vacia, se cerró la conexión */
            close(csock);
            return;
        }

        char reply[MAX_BUF];
        char *token = strtok(buf, " ");
        if (!strcmp(token, "PUT")) { 
			// Introduce al store el valor v bajo la clave k. 
			// El valor viejo para k, si exista, es pisado. El servidor debe responder con OK.
            
			char *key = strtok(NULL, " ");
            char *value = strtok(NULL, " ");
            
            int idx = hash_str(key);

            if (store->hashmap[idx].value[0] != '\0') {
                while (store->hashmap[idx].sig != -1 && strcmp(store->hashmap[idx].key, key)) {
                    idx = store->hashmap[idx].sig;
                }

                if (!strcmp(store->hashmap[idx].key, key)) {
                    strncpy(store->hashmap[idx].value, value, MAX_VALUE);
                    strcpy(reply, "OK");
                }
                else if (store->used < HASHMAP_SIZE) {
                    int newidx = idx;
                    while (store->hashmap[newidx].key[0] != '\0')
                        newidx = (newidx + 1) % HASHMAP_SIZE;

                    store->hashmap[idx].sig = newidx;
                    idx = newidx;
                    strncpy(store->hashmap[idx].key, key, MAX_KEY);
                    strncpy(store->hashmap[idx].value, value, MAX_VALUE);
                    store->hashmap[idx].sig = -1;
                    store->used++;
                    strcpy(reply, "OK");
                }
                else {
                    strcpy(reply, "STOREFULL");
                }
            }
            else {
                strncpy(store->hashmap[idx].key, key, MAX_KEY);
                strncpy(store->hashmap[idx].value, value, MAX_VALUE);
                store->hashmap[idx].sig = -1;
                store->used++;
                strcpy(reply, "OK");
            }
		} else if (!strcmp(token, "DEL")){
			// Borra el valor asociado a la clave k. El servidor debe responder con OK.

			char *key = strtok(NULL, " ");
            int idx = hash_str(key);
            int prv = idx;

            if (store->hashmap[idx].key[0] != '\0') {
                while (idx != -1 && strcmp(store->hashmap[idx].key, key)) {
                    prv = idx;
                    idx = store->hashmap[idx].sig;
                }
                
                if (idx != -1) {
                    store->hashmap[idx].key[0] = '\0';
                    store->used--;

                    int sig = store->hashmap[idx].sig;
                    if (prv != idx)
                        store->hashmap[prv].sig = sig;
                    else if (sig != -1) {
                        store->hashmap[idx] = store->hashmap[sig];
                        store->hashmap[sig].key[0] = '\0';
                    }
                }
            }

            strcpy(reply, "OK");
		} else if (!strcmp(token, "GET")){
			// Busca el valor asociado a la clave k. 
			// El servidor debe contestar con OK v si el valor es v,
			// o con NOTFOUND si no hay valor asociado a k.
            
			char *key = strtok(NULL, " ");
            
            int idx = hash_str(key);
            while (idx != -1 && strcmp(store->hashmap[idx].key, key))
                idx = store->hashmap[idx].sig;
            
            if (idx != -1)
                sprintf(reply, "OK %s", store->hashmap[idx].value);
            else
                strcpy(reply, "NOTFOUND");
		} else {
			// Ante cualquier otro mensaje el servidor responde con EINVAL
            strcpy(reply, "EINVAL");
		}

        int len = strlen(reply);
        reply[len] = '\n';

        send(csock, reply, len+1, 0);
    }
}

void wait_for_clients(int lsock, Store* store)
{
    int csock;

    /* Esperamos una conexión, no nos interesa de donde viene */
    csock = accept(lsock, NULL, NULL);
    if (csock < 0)
        quit("accept");

    int pid = fork();
    if (pid == 0){
        /* Atendemos al cliente */
        handle_conn(csock, store);
        munmap(store, sizeof(Store));
        exit(0);
    }
    else{
        /* Volvemos a esperar conexiones */
        close(csock); // Cerrar en el padre no cierra en el hijo, y sirve para no gastar file descriptors
        wait_for_clients(lsock, store);
    }
}

/* Crea un socket de escucha en puerto 3942 TCP */
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
    sa.sin_port = htons(3942);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bindear al puerto 3942 TCP, en todas las direcciones disponibles */
    rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
    if (rc < 0)
        quit("bind");

    /* Setear en modo escucha */
    rc = listen(lsock, NUM_CLIENTS);
    if (rc < 0)
        quit("listen");

    return lsock;
}

void sigint_handle() {
    unlink(SHM_NAME);
    exit(0);
}

int main()
{
    signal(SIGINT, sigint_handle);

	int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // Crear o abrir la memoria compartida
	ftruncate(fd, sizeof(Store));
	Store* store = mmap(NULL, sizeof(Store), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

    for (int i = 0; i < HASHMAP_SIZE; i++)
        store->hashmap[i].sig = -1, store->hashmap[i].key[0] = '\0';
    store->used = 0;

	int lsock;
	lsock = mk_lsock();
	wait_for_clients(lsock, store);
}
