#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.c"
#include "list.h"

typedef struct {
    char nombre[51];
    char artista[31];
    char **genero;
    int cantGen;
    int anyo;
    int listaR;
}Cancion;

int llenarNombre(Cancion *song, char *linea){
    int i = 0;

    while (1){
        if (linea[i] == ','){
            strncpy(song->nombre, linea, i);
            song->nombre[i] = '\0';
            break;
        }
        i++;
    }
    return i+1;
}

int llenarArtista(Cancion *song, char *linea){
    int i = 0;

    while (1){
        if (linea[i] == ','){
            strncpy(song->artista, linea, i);
            song->artista[i] = '\0';
            break;
        }
        i++;
    }
    return i+1; 
}

int llenarGenero(Cancion *song, char *linea){
    int i = 0;
    
    song->genero = (char**)malloc(sizeof(char*));
    if (song->genero == NULL) exit(1);

    //Si tiene solo un género
    if (linea[i] != '\"'){
        while (1){
            if (linea[i] == ','){
                song->genero[0] = (char*)malloc(sizeof(char));
                strncpy(song->genero[0], linea, i);
                song->genero[0][i] = '\0';
                song->cantGen = 1;
                return i+1;
            }
        i++;  
        }
    }
    
    //Si tiene más de un género
    int fincomilla = 1;
    int cantG = 0;
    while (linea[fincomilla] != '\"'){
        if (linea[fincomilla] == ',') cantG++;
        fincomilla++;
    }
    cantG++;
    song->cantGen = cantG;

    // Reserva de memoria para géneros
    for (int x = 0 ; x < cantG;x++){
        song->genero[x] = (char*)malloc(sizeof(char));
        if (song->genero[x] == NULL) exit(1);
    }

    int cont = 0;
    i = 0;
    linea = (linea+1);
    char limite = ',';

    while (cont < cantG){

        while (1){
            if (linea[i] == limite){
                strncpy(song->genero[cont], linea, i);
                song->genero[cont][i] = '\0';
                linea = (linea+i+1);
                break;
            }
            i++;
        }

        i = 0;
        cont++;
        if (cont == cantG-1) limite = '\"';
    }
    return fincomilla+2;
}

int llenarAnyo(Cancion *song, char *linea){
    int i = 0;
    char *cano = (char*)malloc(sizeof(char));

    while (1){
        if (linea[i] == ','){
            strncpy(cano, linea, i);
            cano[i] = '\0';
            break;
        }
        i++;
    }
    song->anyo = atoi(cano);

    free(cano);
    return i+1;
}

void llenarLista(Cancion *song, char *linea){
    int i = 0;
    char numL[6];

    while (1){
        if (linea[i] == ' ') break;
        i++;
    }
    strcpy(numL, (linea+i+1));
    song->listaR = atoi(numL);
}

void procesarLineaCSV(Cancion *song, char *linea){
    int i;
    char *posI = linea;
    int j;

    //Nombre
    j = llenarNombre(song, posI);
    posI = (posI+j);

    //Artista
    j = llenarArtista(song, posI);
    posI = (posI + j);

    //Genero
    j = llenarGenero(song, posI);
    posI = (posI+j);

    //Año
    j = llenarAnyo(song, posI);
    posI = (posI+j);
    //Lista de Reproducción
    llenarLista(song, posI);
}

void  llenarListaGlobal(Cancion*song, List *cancionesA){
    pushBack(cancionesA ,song->nombre);
    Cancion *aux = lastList(cancionesA);
    printf("%s", aux -> nombre);
    printf("%s", song -> nombre);
}

int main(int argc, char *argv[])
{
    List* cancionesA= createList();
    FILE *archivo = fopen("Canciones.csv", "rt");
    if (archivo == NULL){
        printf("ERROR AL ABRIR ARCHIVO");
        exit(1);
    }
    int lol = 15;
    Cancion array[lol];
    char linea[1024];

    int cont = 0;
    while(cont < lol){
        fgets(linea, 1023, archivo);
        procesarLineaCSV(&array[cont], linea);
        cont++;
        llenarListaGlobal(&array[cont], cancionesA);
    }
    return 0;
}