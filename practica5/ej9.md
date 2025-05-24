Ejercicio 9
Se pide programar el driver para un teclado gamer. Este teclado es muy novedoso, ya que es mucho
más grande y tiene muchas más teclas que los teclados normales, y puede ser utilizado al mismo
tiempo en hasta tres juegos distintos. Esto permite que hasta tres personas a la vez puedan jugar con
el mismo teclado, pero cada quién con su propia aplicación, y sin interferirse entre ellos. Además, el
teclado cuenta con tres visores inteligentes que permiten mostrar información sobre el juego que se
esté jugando (por ejemplo, la vida del jugador, o mensajes importantes). También permite activar,
desactivar y remapear teclas, y configurar macros, para cada usuario por separado. Además de todas
estas impresionantes características, tiene luces de todos los colores posibles (también configurables).
El funcionamiento del controlador del dispositivo es el siguiente: cada vez que el usuario presiona
una tecla el teclado levanta la interrupción IRQ_KEYB y coloca en el registro KEYB_REG_DATA un entero
de 32 bits que contiene en sus 14 bits menos significativos el código KEYCODE correspondiente a la tecla
presionada, y en los 2 bits siguientes un identificador P, que vale 0 cuando la tecla debe ser recibida
por todas las aplicaciones conectadas, y en caso contrario un número entre 1 y 3 que indica a qué
aplicación está destinada esa tecla. Considerar que, al ser una interrupción, esta debe ser atendida
en un tiempo acotado, es decir, no debe bloquearse ni quedarse esperando. Se le deberá informar al
dispositivo que la tecla pudo ser almacenada escribiendo correctamente escribiendo el valor READ_OK
en el registro KEYB_REG_CONTROL. En caso contrario se le deberá escribir el valor READ_FAILED.
Por otro lado, el dispositivo cuenta con tres direcciones de memoria (una por cada aplicación
posible) INPUT_MEM_0, INPUT_MEM_1, INPUT_MEM_2, siendo cada una un arreglo de 100 bytes desde los
que el dispositivo leerá el input de las aplicaciones. Estas direcciones serán mapeadas a memoria por
el driver durante su carga en el Kernel, y se mapearán en el arreglo input_mem.
Cuando una aplicación se conecte al teclado, se le deberá informar al mismo escribiéndole el
valor APP_UP en el registro KEYB_REG_STATUS, y el id de la aplicación correspondiente en el registro
KEYB_REG_AUX. Cuando una aplicación se desconecte se deberá hacer lo mismo pero escribiendo el
valor APP_DOWN.
El driver deberá ir almacenando los caracteres ASCII correspondientes a las distintas pulsaciones
del teclado en un buffer, a la espera de ser leídas por las aplicaciones que se encuentren conectadas. Para
ello, se cuenta con una función keycode2ascii(int keycode) que traduce los códigos de pulsación
en su correspondiente caracter ascii.
Los procesos de usuario que deseen leer el input del teclado deberán hacerlo mediante una ope-
ración de read() bloqueante. Esta operación sólo puede retornar cuando haya leído la cantidad de
bytes solicitados. En caso de haber más de un proceso conectado al dispositivo, cada proceso deberá
poder consumir cada caracter leído de forma independiente. Ejemplo: suponer que hay tres procesos
conectados, y desde el dispositivo se presionaron las teclas correspondientes a ”sistemas”, en todos los
casos con el identificador P=0. En tal caso si p0 hace un read de 2 bytes, obtendrá ”si”, si p1 luego
hace un read de 5 bytes, obtendrá ”siste”, si luego p0 hace un read de 1 byte obtendrá ”s”, y si luego
p2 hace un read de 8 bytes obtendrá ”sistemas”.
Además, los procesos conectados al teclado podrán utilizar la función write() para informarle
al teclado el estado del jugador, comandar los colores del teclado, reconfigurar teclas, y varias otras
funciones que provee el dispositivo.

a) Implementar las funciones driver_load, driver_unload, driver_open, driver_close y
driver_read. Implementar cualquier función o estructura adicional que considere necesaria (tener
en cuenta que en el kernel no existe la libc).
Para resolver la función driver_read es posible implementar una cola circular del modo descripto
a continuación. Las variables buffer_start[i] y buffer_end[i] indican el inicio y el final del buffer.
Al hacer una lectura, la variable start crece. Al hacer una escritura, la variable end crece. Si la
variable end llega al final del buffer, debe comenzar por el principio, siempre teniendo cuidado de no
pisarse con start. Si vale que end+1 es igual a start, entonces se considera que el buffer está lleno.
Para facilitar esta implementación, se cuenta con las funciones, que son todas atómicas:
int get_buffer_length(int i): dado un número de buffer devuelve la cantidad de caracteres
ocupados en el mismo.
boolean write_to_buffer(int i, char src): intenta escribir un caracter en el buffer corres-
pondiente, y retorna un booleano indicando si la escritura se pudo realizar.
boolean write_to_all_buffers(char src): intenta escribir un caracter en todos los buffers,
y retorna un booleano indicando si la escritura se pudo realizar.
void copy_from_buffer(int i, char* dst, int size): lee la cantidad de caracteres indica-
da desde el buffer correspondiente, y los copia en un segundo buffer pasado como parámetro.
Tener en cuenta que esta función no realiza ningún tipo de chequeo sobre los buffers, sino que
simplemente copia.


char input_mem[3][100];
char buffer_lectura[3][1000];
atomic_int buffer_start[3];
atomic_int buffer_end[3];
boolean procesos_activos[3] = {false,false,false};
void driver_load() {
  // Se corre al cargar el driver al kernel.
  // asigno espacios para cada input
  input_mem[0] = INPUT_MEM_0;
  input_mem[1] = INPUT_MEM_1;
  input_mem[2] = INPUT_MEM_2;
  request_irq(IRQ_KEYB, key_handler);
  int read = sem(0); // lo uso cada vez que alguien presiona

  for(int i = 0; i<3; i++ ){
    buffer_start[i] = 0;
    buffer_end[i] = 0;
    procesos_activos[i] = false;
  }
}

void key_handler(){
  read.wait(); // voy a leer, si llegan mas interrupciones espero
  int read = IN(KEYB_REG_DATA);
  int keycode = read[0-13]; // leo los primeros 14 bits
  int p = read[14,15];
  
  char c = keycode2ascii(keycode);

  
  if(p == 0){
   for(int i = 0; i<3;i++){
     if(get_buffer_length(i) == 100){
       // no hay mas lugar en el buffer
       buffer_start[i] = 0;
       buffer_end[i] = 0;
     }
   }
    
   if( write_to_all_buffers(c)){
        OUT(KEYB_REG_CONTROL,READ_FAILED);
   }
  
  }

  else{
    if(get_buffer_length(i) == 100){
      / no hay mas lugar en el buffer
       buffer_start[i] = 0;
       buffer_end[i] = 0;
    }
      if(!write_to_buffer(i,c)) {
        OUT(KEYB_REG_CONTROL,READ_FAILED);
        
      };
     read.signal(); 
  }
  
}

void driver_unload() {
  // Se corre al eliminar el driver del kernel.
}
int driver_open() {
  // Debe conectar un proceso, asignandole un ID y retornandolo
  
  mutex.wait();
  int i = 0;
  while(i < 3){
    if(!procesos_activos[i]) break;
    i++;
  }
  if(i == 3) // si no hay ningun proceso disponible devuelvo -1
    return -1;

  OUT(KEYB_REG_STATUS, APP_UP);
  OUT(KEY_REG_AUX,i);
  procesos_activos[i] = true;
  
  mutex.signal();
  return i;
  // o retornando -1 en caso de falla. 
}
void driver_close(int id) {
  // Debe desconectar un proceso dado por parametro.
  if(procesos_activos[id]){
    procesos_activos[id] = false;
    OUT(KEY_REG_STATUS,APP_DOWN);
    OUT(KEY_REG_AUX,id);
  }  
}
int driver_read(int id, char* buffer, int length) {
  // Debe leer los bytes solicitados por el proceso ’’id’’;
  
  while(buffer_start[id]+length > buffer_end[id]){ // mientras no haya suficiente espacio para leer espero
    if(!procesos_activos[id]) return -1; // se cerro el programa
  }
  copy_from_buffer(id,buffer,length);
  buffer_start[id].inc(length);

  return length;
}
int driver_write(char* input, int size, int proceso) {
  copy_from_user(input_mem[proceso], input, size);
  return size;
}
