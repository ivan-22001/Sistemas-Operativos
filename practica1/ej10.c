#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    int pid_julieta = fork();

    if(pid_julieta == 0){
        write(1,"Soy Julieta\n",12);

        sleep(1);

        if(fork() == 0){
            write(1,"Soy Jennifer\n",14);

            sleep(1);

            exit(0);
        }
        exit(0);
    }
    else{
        write(1,"Soy Juan\n",10);
        sleep(1);

        waitpid(pid_julieta,NULL,0);

        if(fork() == 0){
            write(1,"Soy Jorge\n",10);

            sleep(1);

            exit(0);

        }
        exit(0);
    }
}