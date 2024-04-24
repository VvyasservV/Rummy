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

int randomNumber(){
    srand(time(NULL));
    int random = rand()%5;
    return random;
}
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
    srand(time(NULL));
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

void inicializarJugada(struct ListaDoble* lista) {
    lista->cabeza = NULL;
    lista->cola = NULL;
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
    nuevoJugador->numCartas = 0;
    if (cola->frente == NULL) {
        cola->frente = nuevoJugador;
    } else {
        cola->trasero->siguiente = nuevoJugador;
    }

    cola->trasero = nuevoJugador;

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
            actual->numCartas++;
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
    for (int i = 1; i <= totalJugadores; i++) {
        colorReset();
        printf("Jugador %s: \nFichas:\t", actual->nombre);
        for (int j = 0; j < actual->numCartas; j++) {
            if(isJoker(actual->mano[j].numero))
                printf("%sJ\t", actual->mano[j].color);
            else
                printf("%s %d  ", actual->mano[j].color, actual->mano[j].numero);
        }
        printf("\n");
        actual = actual->siguiente;
    }
}

bool isJoker(int Joker){
    if(Joker == 99)
        return 1;
    return 0;
}

void mezclarJugadores(struct ColaJugadores* cola, int totalJugadores) {
    if (totalJugadores <= 0) {
        printf("Error: totalJugadores es 0 o negativo.\n");
        return; // Si no hay jugadores, no se hace nada
    }

    if (cola->frente == NULL) {
        printf("Error: la cola está vacía.\n");
        return; // Si la cola está vacía, no hay nada que mezclar
    }

    struct Jugador* jugadores[MAX_JUGADORES];
    struct Jugador* actual = cola->frente;

    // Cargar todos los jugadores en un arreglo
    for (int i = 0; i < totalJugadores; i++) {
        jugadores[i] = actual;
        actual = actual->siguiente;
    }

    for (int i = totalJugadores - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Índice aleatorio
        // Intercambiar las posiciones i y j
        struct Jugador* temp = jugadores[i];
        jugadores[i] = jugadores[j];
        jugadores[j] = temp;
    }

    // Reconectar la cola para mantener la circularidad
    for (int i = 0; i < totalJugadores - 1; i++) {
        jugadores[i]->siguiente = jugadores[i + 1];
    }
    jugadores[totalJugadores - 1]->siguiente = jugadores[0]; // Circularidad
    cola->frente = jugadores[0]; // Nuevo frente
    cola->trasero = jugadores[totalJugadores - 1]; // Nuevo trasero

    for (int i = 0; i < totalJugadores; i++) {
        printf("Jugador %d: %s\n", i + 1, jugadores[i]->nombre);
    }
}

void comer(struct ColaJugadores *cola, struct Pila *pila, struct Pila *bote){
    struct Jugador *actual = cola->frente;
    if(pila->top!=0){        
        pila->top--;
        actual->mano[actual->numCartas++] = pila->array[pila->top];
    }else if(bote->top!=0){
        bote->top--;
        actual->mano[actual->numCartas++] = bote->array[bote->top];
    }
}

void descartar(struct ColaJugadores *cola, struct Pila *bote){
    struct Jugador *actual = cola->frente;
    int descartada = rand() % actual->numCartas;
    if(actual->numCartas!=0){
        bote->array[bote->top] = actual->mano[descartada];
        bote->top++; // Aumentar el tope del pozo de descartes

        // Eliminar la carta de la mano del jugador
        for (int i = descartada; i < actual->numCartas - 1; i++) {
            actual->mano[i] = actual->mano[i + 1]; // Desplazar las cartas para llenar el hueco
        }

        actual->numCartas--;
    }
}

void finTurno(struct ColaJugadores *cola) {
    if (cola == NULL || cola->frente == NULL) {
        printf("Error: La cola está vacía.\n");
        return;
    }

    // Avanzar el frente de la cola
    cola->frente = cola->frente->siguiente;
}