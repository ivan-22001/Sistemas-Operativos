Un cronómetro posee 2 registros de E/S:
CHRONO_CURRENT_TIME que permite leer el tiempo medido,
CHRONO_CTRL que permite ordenar al dispositivo que reinicie el contador.
El cronómetro reinicia su contador escribiendo la constante CHRONO_RESET en el registro de control.
Escribir un driver para manejar este cronómetro. Este driver debe devolver el tiempo actual cuando
invoca la operación read(). Si el usuario invoca la operación write(), el cronómetro debe reiniciarse.


int read(){
  int current = IN(CHORNO_CURRENT_TIME);
  reutnr current;
}

void write(){
  OUT(CHRONO_CTRL,CHRONO_RESET);
}
