#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int pid_hijo;
int pid_padre;
int count = 0;

void ping() {
    if (count++ % 3 == 0 && count != 1) {
        char answer = 0;
        do { // No entendi este ciclo raro de joni pero se lo copie
            printf("Continue [y/n]? ");
            answer = tolower(getchar());
            while (getchar() != '\n');
        } while (answer != 'y' && answer != 'n');
        if (answer == 'n') {
            kill(pid_hijo, SIGTERM); // Termino proceso hijo
            exit(0); // Termino proceso padre
        }
    }

    printf("[%d] ping\n", pid_padre);
    kill(pid_hijo, SIGUSR1); // Solicito al proceso hijo que imprima pong
}


void pong(){
    printf("[%d] pong\n",pid_hijo);
    kill(pid_padre,SIGUSR1);
}

int main(){
    pid_padre = getpid();
    pid_hijo = fork();
    if(pid_hijo < 0){
        exit(1);
    }
    else if(pid_hijo == 0){
        pid_hijo = getpid();
        signal(SIGUSR1, pong);
    }
    else{
        sleep(0.5);
        signal(SIGUSR1,pid_padre);
        ping();
    }
    
    while(1);

}