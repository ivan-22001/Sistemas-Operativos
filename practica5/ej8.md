
int driver_init(){
  request_irq(HP_LOW_INK_INT, low); // estan definido este valor  
  request_irq(HP_FINISHED_INT, finish); 
}

void low(){

}

void finish(){
  
} 


int driver_write(int* buffer, int size){

  mutex.wait(); // entro a seccion critica, otro proceso no puede imprimir
  OUT(LOC_TEXT_POINTER, buffer);
  OUT(LOC_TEXT_SIZE,size);
  
  if(IN(LOCK_CTRL) == LOW_INK) {// bajo nivel de ink
    OUT(LOC_STATUS,READY);
    low_ink.wait(5); // espero hasta 5, segun el enunciado, alcanza con probar hasta 5 veces
    // una vez que 
  } 

  OUT(LOCK_CTRL, PRINTING);
  OUT(LOC_STATUS, BUSY);

  printing.wait();

  OUT(LOCK_CTRL, FINISHED);
  OUT(LOC_STATUS,READY);

  mutex.signal();
  return size;
}
