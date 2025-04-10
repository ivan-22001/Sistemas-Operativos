#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

int status, pid, n, start, buffer;

int generate_random_number(){
	return (rand() % 50);
}

void hijoElegido(int i, int start, int pipes[n][2], int pipesPadre[2][2]) {

	int numInicial, numRecibido, numMagico = generate_random_number();
	printf("El random es %d\n", numMagico);

	read(pipesPadre[0][PIPE_READ], &numInicial, sizeof(numInicial));
	
	printf("Soy hijo %d lei numero %d\n", i, numInicial);
	
	if(numInicial > numMagico) {
		write(pipesPadre[1][PIPE_WRITE], &numInicial, sizeof(numInicial));
		exit(0);
	} 

	// Mando el primero
	numInicial++;
	int pipeAnt = i, pipeSuc = (i + 1) % n;
	write(pipes[pipeSuc][PIPE_WRITE], &numInicial, sizeof(numInicial));

	while(read(pipes[pipeAnt][PIPE_READ], &numRecibido, sizeof(numRecibido))) {
		printf("Soy hijo %d lei numero %d\n", i, numRecibido);
		if(numRecibido > numMagico) 
			break;

		numRecibido++;
		write(pipes[pipeSuc][PIPE_WRITE], &numRecibido, sizeof(numRecibido));
	}

	write(pipesPadre[1][PIPE_WRITE], &numRecibido, sizeof(numRecibido));
	exit(0);

}

void hijo(int i, int start, int pipes[n][2], int pipesPadre[2][2]) {
	
	int pipeAnt = i, pipeSuc = (i + 1) % n;

	// Cierro escritura del anterior y lectura del siguiente
	close(pipes[pipeAnt][PIPE_WRITE]);
	close(pipes[pipeSuc][PIPE_READ]);

	for(int i = 0; i < n; i++) {
		if(i != pipeAnt && i != pipeSuc) {
			close(pipes[i][PIPE_WRITE]);
			close(pipes[i][PIPE_READ]);
		}
	}
 
	if(i == start) hijoElegido(i, start, pipes, pipesPadre);

	for(int i = 0; i < 2; i++) {
		close(pipesPadre[i][PIPE_WRITE]);
		close(pipesPadre[i][PIPE_READ]);
	}

	int numRecibido;
	while(read(pipes[pipeAnt][PIPE_READ], &numRecibido, sizeof(numRecibido))) {
		printf("Soy hijo %d lei numero %d\n", i, numRecibido);

		numRecibido++;
		write(pipes[pipeSuc][PIPE_WRITE], &numRecibido, sizeof(numRecibido));
	}
	exit(0);

}

int main(int argc, char **argv) {

	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);
    
	int pipes[n][2], pipePadre[2][2];
	
	// Creo los pipes para comunicar el padre con el elegido
	pipe(pipePadre[0]);
	pipe(pipePadre[1]);
	
	for(int i = 0; i < n; i++) {
		pipe(pipes[i]);
	}
	
    for(int i = 0; i < n; i++) {
		if(fork() == 0) hijo(i, start, pipes, pipePadre);
	}

	// Cierro todos
	for(int i = 0; i < n; i++) {
		close(pipes[i][PIPE_READ]);
		close(pipes[i][PIPE_WRITE]);
	}

	write(pipePadre[0][PIPE_WRITE], &buffer, sizeof(buffer));

	int res;
	read(pipePadre[1][PIPE_READ], &res, sizeof(res));

	printf("Número final recibido: %d\n", res);
}