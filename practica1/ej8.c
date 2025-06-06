#include <stdio.h>

int main(int argc, char const *argv[]){
    int dato = 0;
    int pid = fork();
    //si no hay error, pid vale 0 para el hijo
    //y el valor del process id del hijo para el padre
    if (pid == -1) exit(1);
    //si es -1, hubo un error
    else if (pid == 0) {
        for (int i=0; i< 3; i++) {
            dato++;
            printf("Dato hijo: %d\n", dato);
        }
        exit(0);
    }
    else {
        wait(NULL);
        for (int i=0; i< 3; i++) {
            printf("Dato padre: %d\n", dato);
        }
    }
    exit(0); //cada uno finaliza su proceso
}