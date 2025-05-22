Se desea implementar el driver de una controladora de una vieja unidad de discos ópticos que
requiere controlar manualmente el motor de la misma. Esta controladora posee 3 registros de lectura
y 3 de escritura. Los registros de escritura son:
DOR_IO: enciende (escribiendo 1) o apaga (escribiendo 0) el motor de la unidad.
ARM: número de pista a seleccionar.
SEEK_SECTOR: número de sector a seleccionar dentro de la pista.
Los registros de lectura son:
DOR_STATUS: contiene el valor 0 si el motor está apagado (o en proceso de apagarse), 1 si está
encendido. Un valor 1 en este registro no garantiza que la velocidad rotacional del motor sea la
suficiente como para realizar exitosamente una operación en el disco.
ARM_STATUS: contiene el valor 0 si el brazo se está moviendo, 1 si se ubica en la pista indicada
en el registro ARM.
DATA_READY: contiene el valor 1 cuando el dato ya fue enviado.
Además, se cuenta con las siguientes funciones auxiliares (ya implementadas):
int cantidad_sectores_por_pista(): Devuelve la cantidad de sectores por cada pista del
disco. El sector 0 es el primer sector de la pista.
void escribir_datos(void *src): Escribe los datos apuntados por src en el último sector
seleccionado.
void sleep(int ms): Espera durante ms milisegundos.
Antes de escribir un sector, el driver debe asegurarse que el motor se encuentre encendido. Si no
lo está, debe encenderlo, y para garantizar que la velocidad rotacional sea suficiente, debe esperar
al menos 50 ms antes de realizar cualquier operación. A su vez, para conservar energía, una vez que
finalice una operación en el disco, el motor debe ser apagado. El proceso de apagado demora como
máximo 200 ms, tiempo antes del cual no es posible comenzar nuevas operaciones.
a) Implementar la función write(int sector, void *data) del driver, que escriba los datos apunta-
dos por data en el sector en formato LBA indicado por sector. Para esta primera implementación,
no usar interrupciones.

b) Modificar la función del inciso anterior utilizando interrupciones. La controladora del disco realiza
una interrupción en el IRQ 6 cada vez que los registros ARM_STATUS o DATA_READY toman el valor
1. Además, el sistema ofrece un timer que realiza una interrupción en el IRQ 7 una vez cada 50
ms. Para este inciso, no se puede utilizar la función sleep.

a)
int write(int sector, void *data){
  // 1. Convertir LBA a CHS (pista y sector relativo)
  // use chatgpt porque no se entendia como elegir la pista
  int sectores_por_pista = cantidad_sectores_por_pista();
  int pista = sector / sectores_por_pista;
  int sector_relativo = sector % sectores_por_pista;
  if(!IN(DOR_STATUS)){ 
  // esta apagado, lo enciendo
    OUT(DOR_IO,1); // lo enciendo
    sleep(50); // duermo 50ms hasta que se enciende
  }
  OUT(ARM,pista); // selecciono la pista a usar
  
  while(!IN(ARM_STATUS)){} // espero hasta que llegue a la pista que seleccione en el registro ARM
  
  OUT(SEEK_SECTOR,sector_relativo); // selecciono el sector en la pista

  escribir_datos(data); // escribo en data

  OUT(DOR_IO,0); // apago el motor
  sleep(200); // el apagado demora 200 ms
}


b)
int arm;
int data;
int timer;
int ticks = 0;
int mutex = 1;

void driver_init(){
  request_irq(6,ready);
  request_irq(7,timer);
  arm = sem(0);
  data = sem(0);
  timer = sem(0);
  
}

void ready(){
  if(IN(ARM_STATUS)){
    arm.signal();
  }
  else
   data.signal(); 
}


void timer(){
  if(ticks > 0){
    ticks -= 50;
  }
  else if( ticks == 0)
    timer.signal();
}

int write(int sector, void *data){
  // 1. Convertir LBA a CHS (pista y sector relativo)
  // use chatgpt porque no se entendia como elegir la pista
  mutex.wait();
  
  int sectores_por_pista = cantidad_sectores_por_pista();
  int pista = sector / sectores_por_pista;
  int sector_relativo = sector % sectores_por_pista;
  if(!IN(DOR_STATUS)){ 
  // esta apagado, lo enciendo
    OUT(DOR_IO,1); // lo enciendo
    timer.wait(); // cuando salte la interrupcion salta
  }
  OUT(ARM,pista); // selecciono la pista a usar
  
  arm.wait() // espero hasta que llegue a la pista que seleccione en el registro ARM
  
  OUT(SEEK_SECTOR,sector_relativo); // selecciono el sector en la pista

  escribir_datos(data); // escribo en data

  data.wait(); // espero a que la data se escriba
  
  OUT(DOR_IO,0); // apago el motor
  // deberia hacerlo en un loop pero fiaca
  ticks = 200;
  timer.wait(200); espero 4 veces
  
}


a)
