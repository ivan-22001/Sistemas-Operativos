#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int calcular(const char *expresion) {
    int num1, num2, resultado;
    char operador;

    // Usamos sscanf para extraer los dos números y el operador de la expresión
    if (sscanf(expresion, "%d%c%d", &num1, &operador, &num2) != 3) {
        printf("Formato incorrecto\n");
        return 0;  // En caso de error, retornamos 0.
    }

    // Realizamos la operación según el operador
    switch (operador) {
        case '+':
            resultado = num1 + num2;
            break;
        case '-':
            resultado = num1 - num2;
            break;
        case '*':
            resultado = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
            } else {
                printf("Error: División por cero\n");
                return 0;  // Si hay división por cero, retornamos 0.
            }
            break;
        default:
            printf("Operador no reconocido\n");
            return 0;  // Si el operador no es válido, retornamos 0.
    }

    return resultado;
}

void clienteHandler(int client_socket){

    char buff[255];
    int res;
    while(1){
        recv(client_socket,buff,255,0);
        printf("Recibi del cliente %s\n",client_socket,buff);
        if(strcmp(buff, "exit") == 0) break;
        res = calcular(buff);
        send(client_socket,&res, sizeof(res),0);
    }
    close(client_socket);
    exit(0);
}

int main() {
     
    // COMPLETAR. Este es un ejemplo de funcionamiento básico.
    // La expresión debe ser recibida como un mensaje del cliente hacia el servidor.

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
    
    while(1) {
        // Como accept es bloqueante solo se va a quedar esperando 
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
        // si es mayor a 0 es que la conexion fue exitoso y creo un proceso nuevo para 
        //encargarme de ese cliente
        if(client_socket > 0){
            printf("Se conecto el cliente: %d\n",client_socket);
            if(fork() == 0) clienteHandler(client_socket);
        }
    }
    const char *expresion = "10+5";  
    int resultado = calcular(expresion);

    printf("El resultado de la operación es: %d\n", resultado);
    exit(0);
}

