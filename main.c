#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//Funcion para leer el k-esimo elemento de un string (separado por comas)
const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i]!='\0'){//esta asi predeterminado pero si cambio i+1(predeterminado) por i solo soluciona lista 3 final con un leve cambio

        if(tmp[i]== '\0'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}


int main()
{
    // Se abre el archivo de mundos csv en modo lectura "r"
    FILE *fp = fopen ("Canciones.csv", "r");

    // Cadena para guardar la linea completa del archivo csv
    char linea[1024];
    int i;
    char *aux;
    while (fgets (linea, 1023, fp) != NULL) { // Se lee la linea
        for(i=0;i<5;i++){
            aux = get_csv_field(linea, i); // Se obtiene el nombre
            printf("%s ",aux); 
        }

        printf("\n");
        
    }
    
    return 0;
}
