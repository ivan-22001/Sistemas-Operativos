#include <stdio.h>


void homero(){
  printf("Soy Homero\n");
  if(fork() == 0) {
    printf("Soy Bart\n");
    exit(0);
  }
  wait(NULL);
  if(fork() == 0) {
    printf("Soy Lisa\n");
    exit(0);
  }
  wait(NULL);
  if(fork() == 0) {
    printf("Soy Maggie\n");
    exit(0);
  }
  wait(NULL);
  printf("Homero termino \n");	 
  exit(0);

}

int system(const char* str){
  if(fork() == 0){

    execl("/bin/sh","sh","-c",str,NULL);
  }
  wait(NULL);
  return 0;

}

int main(){

  printf("Soy Abraham\n");
  if(fork() == 0) homero();
  wait(NULL);

  system("ls -l");  
  printf("Abraham termino\n");
  return 0;
}
