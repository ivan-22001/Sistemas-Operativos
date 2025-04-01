#include <stdio.h>    // printf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() pipe() execlp() dup2() close()
#include <sys/wait.h> // wait()

// Constants 0 and 1 for READ and WRITE
enum { READ, WRITE };

// Debe ejecutar "ls -al"  
void ejecutar_hijo_1(int pipe_fd[]) {

    // Se cierra el read del fd
    close(pipe_fd[READ]); 

    // El stdout es el encargado de escribir en el pipe
    dup2(pipe_fd[WRITE],STDOUT_FILENO);
    
    // ya no es necesario el write de fd
    close(pipe_fd[WRITE]);

    // se ejecuta el comando
    execlp("ls","ls -al",NULL);
}
// Debe ejecutar "wc -l"
void ejecutar_hijo_2(int pipe_fd[]) {
    // Se cierra el fd write del hijo 2
    close(pipe_fd[WRITE]);

    // se redirige la lectura al stdin del sistema
    dup2(pipe_fd[READ],STDIN_FILENO);

    // ya no es necesario el fd read
    close(pipe_fd[READ]);

    // como lee del stdin del sistema cuando se ejecute el hijo uno ese output va a ir al stdin
    execlp("wc","wc -l", NULL);
}

int main(int argc, char const* argv[]) {
    int pipe_fd[2];
    pipe(pipe_fd);

    pid_t pid_hijo = fork();

    if(pid_hijo == 0) ejecutar_hijo_1(pipe_fd);
    else{
        pid_t pid_hijo2 = fork();

        if(pid_hijo2 == 0) ejecutar_hijo_2(pipe_fd);
        else{

            // elimino las referencias del padre al fd, sino se queda esperando hasta la eternidad
            close(pipe_fd[READ]);
            close(pipe_fd[WRITE]);

            waitpid(pid_hijo,NULL,0);
            waitpid(pid_hijo2,NULL,0);
        }
    }
    return 0;
}
