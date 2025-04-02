#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define SOCK_PATH "/home/alumno/socket"
#define BACKLOG 10
#define MAXMSG 100
int main() {
    
    struct sockaddr_un srv_name, cli_name;

    srv_name.sun_family = AF_UNIX;
    strcpy(srv_name.sun_path, SOCK_PATH);

    // cli_name.sun_family = AF_UNIX;
    // strcpy(cli_namesun_path, SOCK_PATH);

    socklen_t srvsz = sizeof(srv_name);
    socklen_t clisz = sizeof(cli_name);
    int socket_srv = socket(AF_UNIX, SOCK_STREAM, 0);
    unlink(srv_name.sun_path);
    int result = bind(socket_srv, (struct sockaddr*)&srv_name, srvsz);
    if (result == -1){
        perror("Error");
        exit(1);
    }
    listen(socket_srv, BACKLOG);
   
    char buff[100];
    int socket_cli = accept(socket_srv, (struct sockaddr*)&cli_name, &clisz);
    
    

    int nbytes = recv(socket_cli, buff, MAXMSG, 0);

    printf("SI %s\n", buff);
    strcpy(buff, "PRUEBA 2");
    send(socket_cli, buff, nbytes, 0);

    close(socket_cli);
    
    
    
    close(socket_srv);// no cierro srv antes ?
    remove(SOCK_PATH);
    return 0;
}