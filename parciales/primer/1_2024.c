

proceso1(int fd[][]){
    char buff[255];

    close(fd[1][WRITE]);
    close(fd[1][READ]);

    read(fd[0][READ],buffer,255);
    close(fd[0][READ]);

    char* invertido = invertir(buffer);

    write(fd[0][WRITE], invertido,strlen(invertido)+1);
    close(fd[0][WRITE]);

    exit(0);
}

proceso2(int fd[][]){
    char buff[255];

    close(fd[0][WRITE]);
    close(fd[1][WRITE]);

    read(fd[1][READ],buffer,255); // mensaje original

    char inv[255];

    read(fd[0][READ], inv, 255);

    close(fd[0][READ]);
    close(fd[1][READ]);

    if(strcmp(inv,buffer) == 0){
        printf("Es palindromo\n");
    }
    else{
        printf("No es palindromo\n");
    }
    exit(0);
}

int main(){

    char buffer[255];
    scanf("%s",buffer);
    int pipes[2][2];

    pipe(pipes[0]);    
    pipe(pipes[1]);

    if(fork() == 0){
        sleep(1); // lo duermo un segundo para que el padre pueda escribir primero en el pipe
        proceso1(pipes);
    }
    if(fork() == 0){
        proceso2(pipes);
    }

    write(pipes[0][WRITE],buffer,strlen(buffer)+1);
    write(pipes[1][WRITE],buffer,strlen(buffer)+1);

    close(fd[0][READ]);
    close(fd[1][READ]);

    close(fd[0][WRITE]);
    close(fd[1][WRITE]);

    wait(NULL);
    wait(NULL);

}

