#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int pid_hijo;
int pid_padre;
int count = 0;
int fd[2];
enum {READ, WRITE};


void meMori(){
    wait(NULL);
    exit(0);
}
void ping() {
    if(count >30){
        kill(pid_hijo,SIGKILL);
        exit(0);
    }
    
    if(read(fd[READ],&count,sizeof(int))!=0) // se leyo algo
    {   
        printf("[%d] Padre envia a Hijo el valor: %d\n", pid_padre,count);
        count++;
        
        write(fd[WRITE],&count,sizeof(int));
    } 
    
    kill(pid_hijo, SIGUSR1); // Solicito al proceso hijo que imprima pong
}


void pong(){
    
    if(count > 30){
        exit(0);
    }
    
    if(read(fd[READ],&count,sizeof(int))!=0) // se leyo algo
    {
        printf("[%d] Hijo envia a Padre el valor: %d\n",getpid(),count); 
        count++;
        
        write(fd[WRITE],&count,sizeof(int));
    }
    
    kill(pid_padre,SIGUSR1);
}

int main(){
    pid_padre = getpid();
    pipe(fd);
    pid_hijo = fork();
    if(pid_hijo < 0){
        exit(1);
    }
    else if(pid_hijo == 0){
        signal(SIGUSR1, pong);
    }
    else{
        sleep(0.5);
        signal(SIGUSR1,ping);
        signal(SIGCHLD,meMori);
        printf("[%d] Padre envia a Hijo el valor: %d\n", pid_padre,count);
        count++;
        write(fd[WRITE],&count,sizeof(int));
        kill(pid_hijo, SIGUSR1); 
    }
    
    while(1);

}