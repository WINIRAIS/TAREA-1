#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<locale.h>
#include "list_answer.c"
#include "list.h"

typedef struct {
    char nombre[100];
    char artista[100];
    char **genero;
    int cantGen;
    int anyo;
    char listaR[100];
}Cancion;
void aumentarMemoria(Cancion **vector)
{
    (*vector) = (Cancion *) malloc(sizeof(Cancion));
    if (*vector == NULL) exit(EXIT_FAILURE);
}
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

    while (1){
        if (linea[i] == '\0'){
            strncpy(song->listaR, linea, i+1);
            break;
        }
        i++;
    }
    chomp(song->listaR);
    return;
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
void mostrarCancion(Cancion *song)
{
    printf("%s: \n%s (%d)\n",song->nombre, song->artista, song->anyo);
    int cant = song->cantGen;

    for (int i = 0 ; i < cant ;i++) printf("%s  ",song->genero[i]);
    printf("\nLista: %s\n\n",song->listaR);
}
void mostrarGenero(Cancion* lista){
    printf("Generos :");
    for(int i = 0 ; i < lista->cantGen ; i++){
            printf(" %s ", lista->genero[i]); 
        }
    printf("\n");
}
void mostrarCancion2(List* lista){
    setlocale(LC_CTYPE, "Spanish"); //libreria que limpia los errores de tipeo en español , se supones que deberia mostrar bien una tilde pero no sirve
    Cancion *aux = firstList(lista);

    while (aux != NULL)
    {
        printf("\nCancion : %s\n", aux->nombre);
        printf("Lista de Reproduccion : %s\n", aux->listaR);
        printf("Artista : %s\n", aux->artista);
        printf("Cantidad de Genero : %d\n", aux->cantGen);
        mostrarGenero(aux);
        printf("Anyo : %d\n", aux->anyo);
        aux = nextList(lista);
    }

    return;
}
void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;

    *s = 0;
}
bool cancionExiste(List* lista, Cancion* NuevaCancion){
    Cancion *aux = firstList(lista);
    while (aux != NULL)
    {
        if (strcmp(aux->nombre,NuevaCancion->nombre) == 0){
            if (strcmp(aux->listaR,NuevaCancion->listaR) == 0) return false;
        }
        aux = nextList(lista);
    }
    return true;
}
void Generos(Cancion* Generos){
    Generos->genero = (char**)malloc(Generos->cantGen*sizeof(char*)); 
    if (Generos->genero == NULL) exit(1);
    for(int i = 0 ; i < Generos->cantGen ; i++){
        printf("\nIngrese Genero %ld: ", i+1);
        Generos->genero[i] = (char*)malloc(100*sizeof(char)); if(Generos->genero[i] == NULL) exit(1);
        fflush(stdin);
        fgets(Generos->genero[i], 100, stdin);
        chomp(Generos->genero[i]);
    }
}
void agregarCancion(List* lista){
    Cancion* NuevaCancion = NULL;
    aumentarMemoria(&NuevaCancion);

    printf("\nIngrese Nombre de la cancion : ");
    fflush(stdin);
    fgets(NuevaCancion->nombre, 100, stdin);
    chomp(NuevaCancion->nombre);
    printf("\nIngrese nombre de la Lista de Reproduccion : ");
    fgets(NuevaCancion->listaR, 100, stdin);
    chomp(NuevaCancion->listaR);

    if(cancionExiste(lista,NuevaCancion)){
        printf("\nIngrese Artista de la cancion : ");
        fgets(NuevaCancion->artista, 51, stdin);
        chomp(NuevaCancion->artista);
        printf("\nCuantos Generos tiene su cancion : ");
        scanf("%d", &NuevaCancion->cantGen);
        Generos(NuevaCancion);
        printf("\ningrese el anyo de la cancion : ");
        scanf("%d", &NuevaCancion->anyo);
        printf("\n");
        pushBack(lista,NuevaCancion);
    }else{
        printf("\n***LA CANCION YA EXISTE EN ESA LISTA***\n");
        printf("FAVOR DE INGRESARLA EN UNA LISTA DIFERENTE\n\n");
        free(NuevaCancion);
    }
    return;
}
void EliminarCancion(List* lista){
    char cancion [100];
    int i = 0;
    printf("Que cancion desea eliminar?\n");
    fflush(stdin);
    fgets(cancion,100,stdin);
    chomp(cancion);
    Cancion* aux = firstList(lista);
    while(aux != NULL){
        if(strcmp(aux->nombre, cancion) == 0){
            popCurrent(lista);
            i++;
        }
        aux = nextList(lista);
    }
    if (i > 0) printf("\n***ELIMINACION EXITOSA***\n");
    else printf("\n***NO EXISTE ESA CANCION***\n");
    printf("\n");
}
void buscarNombre(List* lista){
    char cancion [100];
    int i = 0;
    printf("\nINGRESE NOMBRE DE LA CANCION : \n");
    fflush(stdin);
    fgets(cancion,100,stdin);
    chomp(cancion);
    Cancion* aux = firstList(lista);
    while(aux != NULL){
        if(strcmp(aux->nombre, cancion) == 0){
            printf("\nCancion : %s\n", aux->nombre);
            printf("Lista de Reproduccion : %s\n", aux->listaR);
            printf("Artista : %s\n", aux->artista);
            printf("Cantidad de Genero : %d\n", aux->cantGen);
            mostrarGenero(aux);
            printf("Anyo : %d\n", aux->anyo);
            i++;
        }
        aux = nextList(lista);
    }
    if (i > 0) printf("\n***ESOS FUERON LOS RESULTADOS IMPRESOS EXITOSAMENTE***\n");
    else printf("\n***NO EXISTE ESA CANCION***\n");
    printf("\n");
}
void buscarArtista(List* lista){
    char auxArtista [100];
    int i = 0;
    printf("\nINGRESE NOMBRE DEL ARTISTA : \n");
    fflush(stdin);
    fgets(auxArtista,100,stdin);
    chomp(auxArtista);
    Cancion* aux = firstList(lista);
    while(aux != NULL){
        if(strcmp(aux->artista, auxArtista) == 0){
            printf("\nCancion : %s\n", aux->nombre);
            printf("Lista de Reproduccion : %s\n", aux->listaR);
            printf("Artista : %s\n", aux->artista);
            printf("Cantidad de Genero : %d\n", aux->cantGen);
            mostrarGenero(aux);
            printf("Anyo : %d\n", aux->anyo);
            i++;
        }
        aux = nextList(lista);
    }
    if (i > 0) printf("\n***ESOS FUERON LOS RESULTADOS***\n");
    else printf("\n***NO EXISTE ESE ARTISTA***\n");
    printf("\n");
}
void buscarGenero(List* lista){
    char auxGenero [100];
    bool entra = true;
    int i = 0;
    printf("\nINGRESE NOMBRE DEL GENERO : \n");
    fflush(stdin);
    fgets(auxGenero,100,stdin);
    chomp(auxGenero);
    Cancion* aux = firstList(lista);
    while(aux != NULL){
        for(int a = 0 ; a < aux->cantGen ; a++){
            if(entra){
                if(strcmp(aux->genero[a], auxGenero) == 0){
                    printf("\nCancion : %s\n", aux->nombre);
                    printf("Lista de Reproduccion : %s\n", aux->listaR);
                    printf("Artista : %s\n", aux->artista);
                    printf("Cantidad de Genero : %d\n", aux->cantGen);
                    mostrarGenero(aux);
                    printf("Anyo : %d\n", aux->anyo);
                    entra = false;
                    i++;
                }
            }
            if(aux == NULL) break;
        }
        entra = true;
        aux = nextList(lista);
        
    }
    if (i > 0) printf("\n***ESOS FUERON LOS RESULTADOS***\n");
    else printf("\n***NO EXISTE ESE GENERO***\n");
    printf("\n");
}
bool buscarCancion(List* lista){
    int respuesta;
    printf("Buscar por : Nombre (1) | Artista (2) | Genero (3) | Volver al Menu (4)\n");
    scanf("%d", &respuesta);
    if(respuesta == 1) buscarNombre(lista);
    if(respuesta == 2) buscarArtista(lista);
    if(respuesta == 3) buscarGenero(lista);
    if(respuesta == 4) return false;
    return true;
}
bool opcionMenu(List* lista){
    int respuesta;
    printf("Que operacion desea realizar: \n\nAgregar Cancion (1)\nBuscar Cancion (2)\nEliminar Cancion (3)\nMostrar Lista de Canciones (4)\nFinalizar Ejecucion (5)\n\n");
    scanf("%d", &respuesta);    
    if(respuesta == 1){
        agregarCancion(lista);
        return true;
    }
    if(respuesta == 2){
        while(buscarCancion(lista));
        return true;
    }
    if(respuesta == 3){
        EliminarCancion(lista);
        return true;
    }
    if(respuesta == 4){
        mostrarCancion2(lista);
        return true;
    }
    if(respuesta == 5) return false; 
}
int main()
{
    List* listaGlobal = createList();

    FILE *archivo = fopen("Canciones.csv", "rt");
    if (archivo == NULL){
        printf("ERROR AL ABRIR ARCHIVO");
        exit(1);
    }
    
    Cancion* nuevaCancion = NULL;
    char linea[1024];
    
    int cont = 1;
    while(true){
        if(fgets(linea, 1023, archivo) != NULL){
            aumentarMemoria(&nuevaCancion);
            procesarLineaCSV(nuevaCancion, linea);
            pushBack(listaGlobal,nuevaCancion);

        }else break;
    }
    fclose(archivo);
    //Funcion que muestra las opciones para la musica
    while (opcionMenu(listaGlobal));

    return 0;
}            