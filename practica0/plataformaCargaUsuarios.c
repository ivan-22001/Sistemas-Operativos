#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    char *nombre;
    int edad;
    char CUIT[12];
}Usuario;

Usuario* user = NULL;
Usuario** usuarios = NULL;
int size;

bool cuitValido(char* cuit){
    if(strlen(cuit) != 11) return false;
    int count = 0;
    int mult[10] = {5, 4, 3, 2, 7, 6, 5, 4, 3, 2};
    for(int i = 0; i < 10; i++){
        count += (cuit[i] - '0') * mult[i];
    }
    count = count % 11;

    count = 11 - count;
    return (cuit[10] - '0') == count; 
}

void liberarUsuario(){
    if (user != NULL) {
        free(user->nombre);
        free(user);
        user = NULL;
    }
}

void append(Usuario* usuario){
    for(int i = 0; i < size; i++){
        if(usuarios[i] == NULL){
            usuarios[i] = usuario;
            return;
        }
    } 
}
void agregarInformacionUsuario(){

    if(!usuarios){
        usuarios = (Usuario**)malloc(sizeof(Usuario*));
        size = 1;
    }

    user = (Usuario*)malloc(sizeof(Usuario));
    char buffer[100];

    printf("Ingrese nombre de persona:\n > ");
    scanf("%s",buffer);
    user->nombre = strdup(buffer);
            
    printf("Ingrese edad:\n > ");
    scanf("%d",&user->edad);
  

    printf("Ingrese cuit:\n > ");
    scanf("%11s",buffer);
    strcpy(user->CUIT,buffer);

    if((user->edad > 17 && user->edad < 100) && cuitValido(user->CUIT)){
        if(sizeof(usuarios)+1 > size){
            realloc(usuarios, size*2);
            size *= 2;
        }
        append(user);
        printf("Su información fue cargada con éxito!\n");
    }
    else{
        printf("La información cargada es invalida, por favor, intente de nuevo\n");
    }    
}

void verInformacion(){
    if(!usuarios)
        printf("¡Ningún usuario fue cargado a la plataforma todavía!\n");
    else{   
        int i = 0;
        while(usuarios[i] != NULL){
            printf("nombre: %s, edad: %d, cuit: %s \n",usuarios[i] ->nombre,usuarios[i] ->edad,usuarios[i] ->CUIT);
            i++;
        }
    }
}

void buscarInfo(char* cuit){
    for(int i = 0; i < size && usuarios[i] != NULL; i++){
        if(strcmp(cuit,usuarios[i]->CUIT)==0){
            printf("nombre: %s, edad: %d, cuit: %s \n",usuarios[i] ->nombre,usuarios[i] ->edad,usuarios[i] ->CUIT);
            return;
        }
    }
    printf("No existe el usuario\n");
}

int main(){
    
    char input[10000];

    while(1){
        printf("> ");
        scanf("%99s",input);
        if(strcmp(input,"agregarInformacionUsuario")==0){
            agregarInformacionUsuario();
        }
        else if(strcmp(input,"verInformacionUsuario") == 0){
            verInformacion();
        }
        else if(strcmp(input,"buscarInformacion") == 0){
            printf("Ingrese el cuit a buscar: \n > ");
            scanf("%s", input);
            buscarInfo(input);
        }
        else 
            printf("Input invalido, ingrese devuelta\n");
    }
}