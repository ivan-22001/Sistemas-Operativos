#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/select.h>

int main() {
    int server_socket;
    int client_socket;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    socklen_t slen = sizeof(server_addr);
    socklen_t clen = sizeof(client_addr);
    int num = 0;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");
    unlink(server_addr.sun_path);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    bind(server_socket, (struct sockaddr *) &server_addr, slen);
    listen(server_socket, 1);

    // Con esto evito que se bloquee 
    int flags = fcntl(server_socket, F_GETFL, 0);
    fcntl(server_socket, F_SETFL, flags | O_NONBLOCK);
    fd_set ready_sockets, current_sockets;

    FD_ZERO(&current_sockets);
    FD_SET(server_socket,&current_sockets);
    
    printf("Servidor: esperando conexión del cliente...\n");
    int clients[5];
    int max_fd = 5;
    int act = 0;
    while(1) {
        ready_sockets = current_sockets;
        if(select(max_fd,&ready_sockets,NULL,NULL,NULL) < 0){
            perror("select error");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < max_fd; i++){
            if(FD_ISSET(i,&ready_sockets)){
                if(i == server_socket){ // llego una nueva conexion
                    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
                    printf("Se conecto el cliente: %d \n",client_socket);
                    clients[act++] = client_socket;
                    FD_SET(client_socket,&current_sockets);
                }
                else{
                    // revisar para enviar mensajes y recibir
                    char buff[90];
                    recv(i,buff,sizeof(buff),0);
                    for(int j = 0; j < max_fd; j++){
                        if(i != clients[j] && clients[j] = NULL){
                            send(clients[j],buff,sizeof(buff),0);
                        }
                    }
                }
                

            }
        }
    }

    exit(0);
}
