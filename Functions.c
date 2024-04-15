#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif
void colorReset(){
    printf(BLANCO "");
}
void ClearPlayerTurn(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void PCTurn(int i){
    #ifdef _WIN32
        Sleep(i*1000);
        system("cls");
    #else
        sleep(i*1000);
        system("clear");
    #endif
}

void createInitialDeck(struct Fichas Baraja[4][26]) {
    int i;
    for(i = 0; i < 26; i++){
        Baraja[0][i].numero = (i%13)+1;
        Baraja[0][i].color = VERDE;
        Baraja[0][i].asignada = false;
    }
    for(i = 0; i < 26; i++){
        Baraja[1][i].numero = (i%13)+1;
        Baraja[1][i].color = AZUL;
        Baraja[1][i].asignada = false;
    }
    for(i = 0; i < 26; i++){
        Baraja[2][i].numero = (i%13)+1;
        Baraja[2][i].color = NARANJA;
        Baraja[2][i].asignada = false;
    }
    for(i = 0; i < 26; i++){
        Baraja[3][i].numero = (i%13)+1;
        Baraja[3][i].color = ROSA;
        Baraja[3][i].asignada = false;
    }
}

void createJokers(struct Fichas Comodin[2]){
    int i;
    for(i=0;i<2;i++){
        Comodin[i].numero = 99;
        Comodin[i].color = BLANCO;
        Comodin[i].asignada = false;
    }
}

void inicializarCola(struct ColaJugadores *cola) {
    cola->frente = NULL;
    cola->trasero = NULL;
}

void inicializarPila(struct Pila *pila) {
    pila->top = 0; // Inicializar el top de la pila en 0
}


void insertarJugador(struct ColaJugadores *cola, char nombre[], bool esBot) {
    struct Jugador *nuevoJugador = (struct Jugador *)malloc(sizeof(struct Jugador));
    if (esBot) {
        static int numBot = 1;
        sprintf(nuevoJugador->nombre, "Bot %d", numBot++);
    } else {
        strcpy(nuevoJugador->nombre, nombre);
    }
    nuevoJugador->esBot = esBot;
    nuevoJugador->siguiente = NULL;
    
    // Si la cola está vacía
    if (cola->frente == NULL) {
        cola->frente = nuevoJugador;
    } else {
        cola->trasero->siguiente = nuevoJugador;
    }

    // Actualizar el trasero de la cola
    cola->trasero = nuevoJugador;
    // Hacer que la cola sea circular
    cola->trasero->siguiente = cola->frente;
}

void shuffle(struct Fichas *array, int n) {
    if (n > 1) {
        int i;
        for (i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            struct Fichas temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

void repartirCartasYPila(struct ColaJugadores *cola, struct Fichas baraja[4][26],
 struct Fichas comodin[2], int totalJugadores, struct Pila *pila) {
    struct Jugador *actual = cola->frente;
    int cartaPila = 0;
    int totalCartas = 4*26 + 2; // Total de cartas en la baraja, incluyendo los comodines
    int totalManos = totalJugadores; // Total de manos que se repartirán

    // Crear un arreglo temporal para barajar las cartas
    struct Fichas tempBaraja[4 * 26 + 2];
    int idx = 0, k=0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 26; j++) {
            tempBaraja[idx++] = baraja[i][j];
        }
    }
    // Agregar los comodines al arreglo temporal
    tempBaraja[idx++] = comodin[0];
    tempBaraja[idx++] = comodin[1];

    // Barajar el arreglo de cartas
    shuffle(tempBaraja, totalCartas);

    // Repartir las cartas aleatorias a cada jugador
    for (int i = 0; i < totalManos; i++) {
        for (int j = 0; j < 13; j++) { // Cada jugador recibe 13 cartas
            // Verificar si aún quedan cartas en el arreglo barajado
            if (k < totalCartas) {
                actual->mano[j] = tempBaraja[k++];
            } else {
                // Si no quedan más cartas, salir del bucle
                break;
            }
        }
        // Avanzar al siguiente jugador después de repartir todas las cartas de la mano actual
        actual = actual->siguiente; 
    }

    // Llenar la pila con las cartas restantes
    for (; k < totalCartas; k++) {
        pila->array[cartaPila++] = tempBaraja[k];
        pila->top++; // Actualizar el valor de top
    }
}



void imprimirManos(struct ColaJugadores *cola, int totalJugadores) {
    struct Jugador *actual = cola->frente;

    // Imprimir las manos de los jugadores reales
    printf("\nManos de los jugadores:\n");
    for (int i = 1; i <= totalJugadores; i++) {
        colorReset();
        printf("Mano de %s: ", actual->nombre);
        for (int j = 0; j < 13; j++) {
            printf("%s %d  ", actual->mano[j].color, actual->mano[j].numero);
        }
        printf("\n");
        actual = actual->siguiente;
    }
}
