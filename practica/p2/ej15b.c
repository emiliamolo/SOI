#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <pthread.h>
#include <sys/epoll.h>

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

#define THREAD_POOL_N 5
#define MAX_EVENTS 10

void quit(char *s)
{
	perror(s);
	abort();
}

int U = 0;
pthread_mutex_t u_mtx = PTHREAD_MUTEX_INITIALIZER;

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

void handle_conn(int* csock_ptr)
{
	char buf[200];
	int rc;
    int csock = *csock_ptr;

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

            pthread_mutex_lock(&u_mtx);
			sprintf(reply, "%d\n", U);
			U++;
            pthread_mutex_unlock(&u_mtx);
            
			write(csock, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			close(csock);
			return;
		}
	}
}

void epoll_wait_for_clients(void **args) {
	int epollfd = *(int*)args[0], lsock = *(int*)args[1];
	struct epoll_event events[MAX_EVENTS];

	while(1) {
		int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1)
			quit("epoll_wait");
		
		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == lsock) {
				int csock = accept(lsock, NULL, NULL);
				if (csock == -1)
					quit("accept");

				setnonblocking(csock);
				
				struct epoll_event ev;
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = csock;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, csock, &ev) == -1)
					quit("epoll_ctl: csock");
			}
			else {
				handle_conn(events[i].data.fd);
			}
		}
	}
}

void wait_for_clients(int lsock)
{
	struct epoll_event ev;

	int epollfd = epoll_create1(0);
	if (epollfd == -1)
		quit("epoll_create1");
	
	ev.events = EPOLLIN; // Esperar a conexiones entrantes (lectura / read)
	ev.data.fd = lsock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, lsock, &ev) == -1)
		quit("epoll_ctl: lsock");

	pthread_t thread_pool[THREAD_POOL_N];

	void *args[] = {&epollfd, &lsock};
	for (int i = 0; i < THREAD_POOL_N; i++) 
		pthread_create(&thread_pool[i], NULL, (void* (*)(void*))epoll_wait_for_clients, args);
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

int main()
{
	int lsock;
	lsock = mk_lsock();
	wait_for_clients(lsock);
}
