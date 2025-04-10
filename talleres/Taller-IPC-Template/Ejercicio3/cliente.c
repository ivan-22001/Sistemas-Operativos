#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
int server_socket;

void handler(){
    printf("Me retiro señores\n");
    write(server_socket, "exit" ,strlen("exit")+1);
    close(server_socket);
    exit(0);
}

int main() {
    struct sockaddr_un server_addr;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");


    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error");
        exit(1);
    }

    signal(SIGINT,handler);
  	char buff[255];
	int n;
    while(1){
		scanf("%s",buff);
		if(strcmp(buff,"exit") == 0) break;
        printf("Le envio al servidor: %s \n", buff);
        write(server_socket, buff,strlen(buff)+1);
        read(server_socket, &n, sizeof(n));
		printf("El resultado de la operacion es: %d \n",n);
    }

    close(server_socket);
    exit(0);
}
