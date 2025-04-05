#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int server_socket;
    int client_socket;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    u_int16_t slen = sizeof(server_addr);
    u_int16_t clen = sizeof(client_addr);
    int num = 0;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");
    unlink(server_addr.sun_path);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    bind(server_socket, (struct sockaddr *) &server_addr, slen);
    listen(server_socket, 1);

    printf("Servidor: esperando conexión del cliente...\n");
    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
    while(1) {
        
        printf("Proceso1 envia %d al Proceso2\n", num);
        send(client_socket, &num, sizeof(num), 0);
        sleep(1);
        recv(client_socket, &num, sizeof(num), 0);
        num++;
    }

    exit(0);
}
