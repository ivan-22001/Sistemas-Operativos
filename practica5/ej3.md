Una tecla posee un único registro de E/S : BTN_STATUS. Solo el bit menos significativo y el segundo
bit menos significativo son de interés:

BTN_STATUS0: vale 0 si la tecla no fue pulsada, 1 si fue pulsada.
BTN_STATUS1: escribir 0 en este bit para limpiar la memoria de la tecla.

Escribir un driver para manejar este dispositivo de E/S. El driver debe retornar la constante
BTN_PRESSED cuando se presiona la tecla. Usar busy waiting.

void driver_init(){
  mutex = sem(1);
}

int driver_read(int* data){

  
  mutex.wait();
  while(IN(BTN_STATUS_0) != 1); // quedo esperando hasta que presione la tecla el usuario
  OUT(BTN_STATUS_1,0); // limpio la memoria de la tecla
  // Cuando hago 0 o 1 me refiero a esos bits, osea out va a escribir solo en el bit 1, no en los demas
  mutex.signal()
  
  int pressed = BTN_PRESSED;
  copy_to_user(data,&pressed,sizeof(pressed));
  return pressed;
}
