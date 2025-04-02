#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define SOCK_PATH "/home/alumno/socket"
#define BACKLOG 10
#define MAXMSG 100
int main(){
    struct sockaddr_un srv_name;

    srv_name.sun_family = AF_UNIX;
    strcpy(srv_name.sun_path, SOCK_PATH);

    socklen_t srvsz = sizeof(srv_name);

    int socket_srv = socket(AF_UNIX, SOCK_STREAM, 0);
    
    printf("Conecto %d\n", connect(socket_srv, (struct sockaddr*)&srv_name, srvsz));
    // no hace falta hacer bind ya de por si es doble sentido
    char buf[MAXMSG+1];
    strcpy(buf, "Prueba");
    
    send(socket_srv, buf, MAXMSG, 0);
    
    recv(socket_srv, buf, MAXMSG, 0);
    
    printf("Recibi %s\n", buf);
    
    return 0;
}