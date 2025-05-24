
int driver_init(){
  request_irq(HP_LOW_INK_INT, low); // estan definido este valor  
  request_irq(HP_FINISHED_INT, finish); 
  int mutex = sem(1);
  int printing = sem(0);
  int low_ink = sem(0);
}

void low(){

}

void finish(){
  printing.signal();  
} 


int driver_write(int* buffer, int size){

  mutex.wait(); // entro a seccion critica, otro proceso no puede imprimir
  OUT(LOC_TEXT_POINTER, buffer);
  OUT(LOC_TEXT_SIZE,size);
  OUT(LOC_CTRL,START);

  for(int i = 0; i < 5; i++){ //pruebo 5 veces
     if(IN(LOCK_CTRL) != LOW_INK) break;  
  }
 

  OUT(LOCK_CTRL, PRINTING);
  OUT(LOC_STATUS, BUSY);

  printing.wait();

  OUT(LOCK_CTRL, FINISHED);
  OUT(LOC_STATUS,READY);

  mutex.signal();
  return size;
}
