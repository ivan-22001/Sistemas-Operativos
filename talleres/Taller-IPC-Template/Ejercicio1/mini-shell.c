#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

void hijo(int k, int count, int pipes[count - 1][2], char** program) {
	
	/*
	printf("SOY %d\n", k);
	int i = 0;
	while(program[i] != NULL) {
		printf("Program es %s\n", program[i]);
		i++;
	}
	*/

	if(k < count - 1) {
		// Cierro lectura y conecto escritura a stdout (salida estándar)
		close(pipes[k][PIPE_READ]);
		dup2(pipes[k][PIPE_WRITE], STDOUT_FILENO);
	}
	
	if(k > 0) {
		// Cierro escritura y conecto lectura a stdin (entrada estándar)
		close(pipes[k - 1][PIPE_WRITE]);
		dup2(pipes[k - 1][PIPE_READ], STDIN_FILENO);
	}
	 
	// Cierro referencias sobrantes
	for(int i = 0; i < count; i++) {
		if(i != k && i != k + 1) {
			close(pipes[i][PIPE_READ]);
			close(pipes[i][PIPE_WRITE]);
		}
	}

	// Ejecuto
	execvp(program[0], program);
		
}

static int run(char ***progs, size_t count) {	
	
	int r, status;

	// Reservo memoria para el arreglo de pids
	pid_t *children = malloc(sizeof(*children) * count);
	
	int pipes[count - 1][2];

	for(int i = 0; i < count - 1; i++) {
		int p = pipe(pipes[i]);
	}

	for(int i = 0; i < count; i++) {
		pid_t nuevoHijo = fork();
		if(nuevoHijo == 0) {
			hijo(i, count, pipes, progs[i]);
		}
		children[i] = nuevoHijo;
	}
	
	// Cierro todos los pipes del padre
	for(int i = 0; i < count - 1; i++) {
		close(pipes[i][PIPE_READ]);
		close(pipes[i][PIPE_WRITE]);
	}

	//Espero a los hijos y verifico el estado que terminaron
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status, 0);

		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;
	free(children);

	return r;
}


int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("El programa recibe como parametro de entrada un string con la linea de comandos a ejecutar. \n"); 
		printf("Por ejemplo ./mini-shell 'ls -a | grep anillo'\n");
		return 0;
	}
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}