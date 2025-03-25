#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

struct Usuario{
    char nombre[100];
    int edad;
    char CUIT[11];
};

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

int main(){

    struct Usuario* user = NULL;
    char input[10000];

    while(1){
        printf("> ");
        scanf("%99s",input);
        if(strcmp(input,"agregarInformacionUsuario")==0){
            char nombre[100];
            int edad;
            char CUIT[11];
            printf("Ingrese nombre de persona:\n > ");
            scanf("%s",nombre);
            
            printf("Ingrese edad:\n > ");
            scanf("%d",&edad);
            while (getchar() != '\n');

            printf("Ingrese cuit:\n > ");
            scanf("%s",CUIT);
            if((edad > 17 && edad < 100) && cuitValido(CUIT)){
                user = malloc(sizeof(struct Usuario));
                strcpy(user->CUIT,CUIT);
                user->edad = edad;
                strcpy(user->nombre,nombre);
                printf("Su información fue cargada con éxito!\n");
            }
            else{
                printf("La información cargada es invalida, por favor, intente de nuevo\n");
            }
        }
        else if(strcmp(input,"verInformacionUsuario") == 0){
            if(!user)
                printf("¡Ningún usuario fue cargado a la plataforma todavía!\n");
            else{
                printf("nombre: %99s, edad: %d, cuit: %s \n",user->nombre,user->edad,user->CUIT);
            }
        }
        else 
            printf("Input invalido, ingrese devuelta\n");
    }
}