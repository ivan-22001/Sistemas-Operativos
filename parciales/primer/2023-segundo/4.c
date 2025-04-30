
/*
Desde el Departamento de Computaci´on se inauguraron m laboratorios nuevos y necesitamos desarrollar un nuevo sistema
para que n estudiantes sepan cu´al n´umero de laboratorio le corresponde. La asignaci´on se hace de manera aleatoria. Se
supone que esta aleatoriedad permitir´a una distribuci´on uniforme de los labos.
Para eso se pide desarrollar un sistema utilizando comunicaci´on entre procesos mediante pipes y que cumpla con
el siguiente criterio: usando programaci´on en el lenguaje C, crear un c´odigo que ejecute un proceso (padre) que se
encargar´a de pedirle a una cantidad n de procesos hijos (el n´umero n ser´a pasado como par´ametro, y representa el
n´umero de estudiantes) que calculen qu´e laboratorio le corresponde a cada une. Una vez hecho eso, cada proceso hijo
tendr´a que comunicarle al padre el n´umero de laboratorio que le corresponde. La forma en que cada proceso asigna un
laboratorio es aleatoria, usando como semilla el id del proceso hijo. Para esto, el padre debe informarle a sus hijos cu´antos
laboratorios nuevos hay (el n´umero m, pasado como par´ametro en la entrada est´andar del padre). Al finalizar, el padre
debe imprimir los n´umeros de asignaci´on de aula a cada proceso por salida est´andar, mostrando el id del proceso hijo y
el n´umero del labo asignado.
a) Realizar el c´odigo en C que cumpla los requisitos de la comunicaci´on. Justificar detalladamente la decisi´on que se
toma en cuanto al cierre de pipes.
b) Se pide modificar el c´odigo anterior para realizar el siguiente comportamiento. Nos piden que, una vez asignados
los laboratorios, el c´odigo del padre ya no las imprima por pantalla. En cambio se debe ejecutar un programa que
nos proveen llamado “nuevaSalida”. Este programa ejecutable toma como par´ametros por entrada est´andar el id de
un proceso y el n´umero de laboratorio asignado y lo imprime en pantalla (salida est´andar), sin embargo, queremos
que el resultado de este programa no sea escrito en el STDOUT sino en un archivo llamado “resultados.out” en
donde guardaremos un log de la ejecuci´on. Justifique la pol´ıtica de cierres de pipes.
Ayuda: Pueden usar las siguientes funciones auxiliares:
int open(char* archivo, O_WRONLY) // abre el archivo en modo de escritura, retornando el descriptor
correspondiente.
int dameLabo(int process_id, m) // toma el process_id correspondiente a un proceso hijo de un
alumno y el n´umero de laboratorios nuevos "m" y devuelve aleatoriamente un n´umero de labo
asignado, entre 1 y m, usando como semilla el process_id enviado.
*/
// a)
void child(int fd[], int m){

    int labo = dameLabo(m);
    write(fd[WRITE],&labo,sizeof(labo));
    exit(0);
}

int labos(int n,int m){

    int pipes[n][2];
    pid_t children[n];
    for(int i = 0, i < n; i++){
        if(pipe(pipes[i]) < 0){ 
            fprintf(STDERR_FILENO, "error creando pipe\n");
            exit(EXIT_FAILURE);
        }

        children[i] = fork();
        if(children[i] == 0){ 
            // soy el hijo
            close(pipes[i][READ]); // cierro el pipe de read porque el hijo no lee
            child(pipes[i],m);
        }

    }

    for(int i = 0; i < n ; i++){
        int labo;
        read(pipes[i][READ],&labo,sizeof(labo));
        printf("Hijo %d se le asigno el labo %d",children[i],labo);
    }   

}

// b) en este modificamos la funcion labo

int labos(int n,int m){

    int pipes[n][2];
    pid_t children[n];
    for(int i = 0, i < n; i++){
        if(pipe(pipes[i]) < 0){ 
            fprintf(STDERR_FILENO, "error creando pipe\n");
            exit(EXIT_FAILURE);
        }

        children[i] = fork();
        if(children[i] == 0){ 
            // soy el hijo_
            close(pipes[i][READ]); // cierro el pipe de read porque el hijo no lee
            child(pipes[i],m);
        }

    }

    int fd_out = open("resultados.out", O_WRONLY);
    dup2(fd_out, STDOUT_FILENO); // ahora el standar output apunta al fd del archivo, el cual se escribe todo
    for(int i = 0; i < n ; i++){
        int labo;
        read(pipes[i][READ],&labo,sizeof(labo));
        wait(NULL);
        int fd_in[2]; // file descriptor para pasarle al hijo 
        
        if(fork() == 0){

            dup2(fd_in[READ],STDIN_FILENO); //  por el enunciado, el programa le llegan los parametros por el estandar input
            close(fd_in[WRITE]); // cierro el write ya que no se va a usar, solo nos importa que read;

            char* nuevaSalida = {"nuevaSalida",NULL};
            execvp(nuevaSalida[0],nuevaSalida);
        }

        // escribo por el pipe el id y el labo asignado, esto le llega al stdin 
        write(fd_in[WRITE],&labo,sizeof(labo)); 
        write(fd_in[WRITE],&children[i],sizeof(int));

        close(fd_in[WRITE]);
        close(fd_in[READ]);

        wait(NULL);
    }   
    exit(0);
}
