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
#include <errno.h>
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

#define KEY_SIZE 32
#define VALUE_SIZE 256
#define MAX_KEYS 100
#define SHM_NAME "/shm_key_value_store" 
/* la memoria compartida creada con shm_open se representa como un archivo
 especial en el directorio /dev/shm/*/

typedef struct {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
}KeyValue;

typedef struct{
	KeyValue store[MAX_KEYS];
	int store_size;
}Store;

void quit(char *s)
{
	perror(s);
	abort();
}

int U = 0;

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


void handle_conn(int csock, Store* store)
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

		char reply[300];
		char *token = strtok(buf, " ");
		if (!strcmp(token, "PUT")){ 
			//introduce al store el valor v bajo la clave k. 
			//El valor viejo para k, si exista, es pisado. El servidor debe responder con OK.
			
			token = strtok(NULL, " ");
			int i = 0;
			while(i<store->store_size && strcmp(token, store->store[i].key) != 0) i++;
			
			if (store->store_size >= MAX_KEYS) {
 			   sprintf(reply, "STORE_FULL\n");
			}else if (i == store->store_size){ // no estaba, la agrego
				strcpy(store->store[i].key, token);
				token = strtok(NULL, "\n");
				strcpy(store->store[i].value, token);
				store->store_size++;
				sprintf(reply, "OK\n");
			}
			else if (!strcmp(token, store->store[i].key)){ // si estaba lo sobreescribo
				token = strtok(NULL, "\n");
				strcpy(store->store[i].value, token);
				sprintf(reply, "OK\n");
			}
			write(csock, reply, strlen(reply));
		} else if(!strcmp(token, "DEL")){
			// Borra el valor asociado a la clave k. El servidor debe responder con OK.
			token = strtok(NULL, " ");
			int i = 0;
			while(i<store->store_size && strcmp(token, store->store[i].key) != 0) i++;
			if (store->store_size == 0 || i == store->store_size)
				sprintf(reply, "NOTFOUND\n");
			else if (!strcmp(token, store->store[i].key)){
				store->store_size--;
				if (store->store_size > 0){
					strcpy(store->store[i].key, store->store[store->store_size].key);
					strcpy(store->store[i].value, store->store[store->store_size].value);	
				}
				sprintf(reply, "OK\n");
			}
			write(csock, reply, strlen(reply));
		}else if(!strcmp(token, "GET")){
			// Busca el valor asociado a la clave k. 
			// El servidor debe contestar con OK v si el valor es v,
			// o con NOTFOUND si no hay valor asociado a k.
			token = strtok(NULL, " ");
			int i = 0;
			while(i< store->store_size && strcmp(token, store->store[i].key) != 0) i++;
			if (i == store->store_size)
				sprintf(reply, "NOTFOUND\n");
			else if (!strcmp(token, store->store[i].key))
				sprintf(reply, "OK %s\n", store->store[i].value);
			write(csock, reply, strlen(reply));
		} else {
			// Ante cualquier otro mensaje el servidor responde con EINVAL
			sprintf(reply, "Error: EINVAL (%d)\n", EINVAL);
			write(csock, reply, strlen(reply));
		}
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
        exit(0);
    }
    else{
        /* Volvemos a esperar conexiones */
		close(csock);
	    wait_for_clients(lsock, store);
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
	sa.sin_port = htons(3942);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 3942 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

int main()
{
	int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // Crear o abrir la memoria compartida
	ftruncate(fd, sizeof(Store)); // no deberia hacer falta si borras bien la memoria compartida con shm_unlink, pero no lo estoy haciendo
	Store* store = mmap(NULL, sizeof(Store), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	store->store_size = 0;
	close(fd);
	int lsock;
	lsock = mk_lsock();
	wait_for_clients(lsock, store);
}
