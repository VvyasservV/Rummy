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
    nuevoJugador->jugadorActivo = false;
    if (cola->frente == NULL) {
        cola->frente = nuevoJugador;
    } else {
        cola->trasero->siguiente = nuevoJugador;
    }

    cola->trasero = nuevoJugador;

    cola->trasero->siguiente = cola->frente;
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
        for (int j = 0; j < 14; j++) { // Cada jugador recibe 14 cartas
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

void imprimirIndices(struct Jugador *actual){
    printf("%s", BLANCO);
    for (int j = 0; j < actual->numCartas; j++) {
        printf("%d\t",(j+1));
    }
}


void imprimirManoActual(struct Jugador *actual){
    for (int j = 0; j < actual->numCartas; j++) {
        if(isJoker(actual->mano[j].numero))     printf("%sJ\t", actual->mano[j].color);
        else    printf("%s%d\t", actual->mano[j].color, actual->mano[j].numero);
        if(j != 0 && j%15==0)   printf("\n");
    }
}

void imprimirManos(struct ColaJugadores *cola, int totalJugadores) {
    struct Jugador *actual = cola->frente;

    // Imprimir las manos de los jugadores reales
    for (int i = 1; i <= totalJugadores; i++) {
        colorReset();
        printf("%s: \nFichas:\t", actual->nombre);
        if(actual == cola->frente){
            imprimirManoActual(actual);
        }else{
            for (int j = 0; j < actual->numCartas; j++) {
                printf("%s*\t", GRIS);
                if(j != 0 && j%15==0)
                    printf("\n");
            }
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

int revisarJugada(struct Fichas fichas[MAX_JUGADA], int arrSize) {
    int corridas = 0, OAK = 0;
    
    // Ordena el arreglo para verificar las corridas
    ordenarMano(fichas, arrSize);
    
    // Verifica corridas
    for (int i = 1; i < arrSize; i++) {
        if (fichas[i - 1].numero < fichas[i].numero 
            && strcmp(fichas[i - 1].color, fichas[i].color) == 0) {  // Mismo color
            corridas = 1;
        }else   corridas = 0;
    }
    
    // Verifica "OAK" (Tres iguales)
    for (int i = 1; i < arrSize; i++) {
        if (fichas[i - 1].numero == fichas[i].numero
            && strcmp(fichas[i - 1].color, fichas[i].color) != 0) {
            OAK = 1;
        }else   OAK = 0;
    }
    
    return corridas || OAK;  // Devuelve 1 si hay corridas o "OAK", de lo contrario 0
}
void comer(struct ColaJugadores *cola, struct Pila *pila, struct Pila *bote){
    struct Jugador *actual = cola->frente;
    int opcion = 0;
    if(actual->esBot == 1){
        if(pila->top!=0){        
            pila->top--;
            actual->mano[actual->numCartas++] = pila->array[pila->top];
        }else if(bote->top!=0){
            bote->top--;
            actual->mano[actual->numCartas++] = bote->array[bote->top];
        }
    } else if(actual->esBot == 0){
        if(pila->top > 0 || bote->top > 0){
            if(pila->top > 0 && bote->top > 0){
                while(opcion != 1 && opcion != 2){ // Cambiado 'option' a 'opcion'
                    printf("1....Comer del pozo\n");
                    printf("2....Comer del pozo de descartes\n");
                    scanf("%d", &opcion);
                    if(opcion != 1 && opcion != 2){
                        printf("Opcion incorrecta, intentalo de nuevo\n");
                        PCTurn(3);
                    }
                }
                if(opcion == 1){        
                    pila->top--; 
                    actual->mano[actual->numCartas++] = pila->array[pila->top];
                } else if(opcion == 2){
                    bote->top--; 
                    actual->mano[actual->numCartas++] = bote->array[bote->top];
                }
            } else if(pila->top > 0){        
                pila->top--; 
                actual->mano[actual->numCartas++] = pila->array[pila->top];
            } else if(bote->top > 0){ 
                bote->top--; 
                actual->mano[actual->numCartas++] = bote->array[bote->top];
            }
        } else {
            printf("No puedes comer!\n");
        }
    }
}

void descartar(struct ColaJugadores *cola, struct Pila *bote){
    struct Jugador *actual = cola->frente;
    int descartada=-1;
    if(actual->esBot == 0){
        while(descartada > actual->numCartas || descartada < 0){
            printf("Selecciona el indice de la ficha a descartar\n");
            imprimirManoActual(actual);
            printf("\n");
            imprimirIndices(actual);
            printf("\n");
            scanf("%d", &descartada);
            descartada--;
            if(descartada < 0 && descartada > actual->numCartas){
                printf("Indice incorrecto, ingresa un indice valido\n");
                PCTurn(2);
            }
        }
        if(actual->numCartas!=0){
            bote->array[bote->top] = actual->mano[descartada];
            bote->top++; // Aumentar el tope del pozo de descartes

            // Eliminar la carta de la mano del jugador
            for (int i = descartada; i < actual->numCartas - 1; i++) 
                actual->mano[i] = actual->mano[i + 1]; // Desplazar las cartas para llenar el hueco
            actual->numCartas--;
        }        
    }else if(actual->esBot == 1){
        descartada=rand() % actual->numCartas;
        if(actual->numCartas!=0){
            bote->array[bote->top] = actual->mano[descartada];
            bote->top++; // Aumentar el tope del pozo de descartes

            // Eliminar la carta de la mano del jugador
            for (int i = descartada; i < actual->numCartas - 1; i++) 
                actual->mano[i] = actual->mano[i + 1]; // Desplazar las cartas para llenar el hueco
            actual->numCartas--;
        }
    }
    
}

void agregarJugada(struct Tablero *tablero,struct CDLL *nueva_lista) {
    struct NodoTablero *nuevo_nodo = (struct NodoTablero *)malloc(sizeof(struct NodoTablero));
    nuevo_nodo->lista = nueva_lista;

    if (tablero->cabeza == NULL) {
        tablero->cabeza = nuevo_nodo;
        nuevo_nodo->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = nuevo_nodo;
    } else {
        struct NodoTablero *ultimo = tablero->cabeza->anterior;
        ultimo->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = ultimo;
        nuevo_nodo->siguiente = tablero->cabeza;
        tablero->cabeza->anterior = nuevo_nodo;
    }
    tablero->tamanio++;
}

void ordenarMano(struct Fichas *fichas[MAX_JUGADA], int n) {

    if (n <= 1) {
        return;  // No hay nada que ordenar si solo hay una carta o ninguna
    }

    for (int i = 1; i < n; i++) {
        struct Fichas key = *fichas[i];  // Ahora es una copia, no un puntero
        int j = i - 1;

        while (j >= 0 && n > key.numero) {
            *fichas[j + 1] = *fichas[j];  // Desplaza el elemento
            j--;  // Mueve el índice hacia la izquierda
        }

        // Inserta la ficha en la posición correcta
        *fichas[j + 1] = key;
    }
}


void agregarFichaPorDerecha(struct CDLL *jugada, struct Fichas valor) {
    struct Nodo *nuevo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevo_nodo->ficha = valor;

    if (jugada->cabeza == NULL) {
        jugada->cabeza = nuevo_nodo;
        jugada->cabeza->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
    } else {
        struct Nodo *cola = jugada->cabeza->anterior;
        cola->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = cola;
        nuevo_nodo->siguiente = jugada->cabeza;
        jugada->cabeza->anterior = nuevo_nodo;
    }
    jugada->tamanio++;
}

void agregarFichaPorIzquierda(struct CDLL *jugada, struct Fichas valor) {
    struct Nodo *nuevo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevo_nodo->ficha = valor;

    if (jugada->cabeza == NULL) {
        jugada->cabeza = nuevo_nodo;
        jugada->cabeza->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
    } else {
        struct Nodo *cola = jugada->cabeza->anterior;
        nuevo_nodo->siguiente = jugada->cabeza;
        nuevo_nodo->anterior = cola;
        cola->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
        jugada->cabeza = nuevo_nodo;
    }
    jugada->tamanio++;
}

struct Fichas robarPorInicio(struct CDLL *jugada) {
    if (jugada->cabeza == NULL) {
        printf("Lista vacía, no se puede robar por inicio.\n");
        return (struct Fichas){-1, "N/A", false}; // Indicador de error
    }

    struct Nodo *robar_nodo = jugada->cabeza;
    struct Fichas resultado = robar_nodo->ficha;

    if (jugada->tamanio == 1) {
        jugada->cabeza = NULL;
    } else {
        jugada->cabeza = robar_nodo->siguiente;
        jugada->cabeza->anterior = robar_nodo->anterior;
        robar_nodo->anterior->siguiente = jugada->cabeza;
    }

    free(robar_nodo);
    jugada->tamanio--;

    return resultado;
}

struct Fichas robarPorFinal(struct CDLL *jugada) {
    if (jugada->cabeza == NULL) {
        printf("Lista vacía, no se puede robar por final.\n");
        return (struct Fichas){-1, "N/A", false}; // Indicador de error
    }

    struct Nodo *ultimo = jugada->cabeza->anterior;
    struct Fichas resultado = ultimo->ficha;

    if (jugada->tamanio == 1) {
        jugada->cabeza = NULL;
    } else {
        ultimo->anterior->siguiente = jugada->cabeza;
        jugada->cabeza->anterior = ultimo->anterior;
    }

    free(ultimo);
    jugada->tamanio--;

    return resultado;
}

int iniciarJugada(struct Jugador *actual, int indices[8][MAX_JUGADA], int i){
    int finJugada = 0, indiceSeleccionado = 0, j = 0, sumaIndices = 0, jugadaValida, arrSize; 
    struct Fichas posibleJugada[MAX_JUGADA];
    while(finJugada != 2 && j<MAX_JUGADA){
        indiceSeleccionado = 0;
        printf("Selecciona los indices de las cartas a jugar\n");
        imprimirManoActual(actual);
        printf("\n");
        imprimirIndices(actual);
        printf("\n");
        while(indiceSeleccionado > (actual->numCartas)+1 || indiceSeleccionado < 1){
            scanf("%d", &indiceSeleccionado);
            if(indiceSeleccionado > (actual->numCartas)+1 || indiceSeleccionado < 1){
                printf("Dato invalido, ingresa otro indice\n");
            }
        }
        indices[i][j] = indiceSeleccionado - 1;
        j++;
        while(finJugada != 1 && finJugada != 2){
            printf("Deseas agregar otro indice?\n");
            printf("1...Si\n");
            printf("2...No\n");
            scanf("%d", &finJugada);
            if(finJugada != 1 && finJugada != 2){
                printf("Dato invalido, ingresa otra opcion\n");
            }
        }
        if(finJugada == 2 && j < 3){
            printf("Faltan fichas para crear un juego valido\n");
            printf("Estas seguro de querer salir?\n");
            while(finJugada != 1 && finJugada != 2){
                printf("1...No\n");
                printf("2...Si\n");
                scanf("%d", &finJugada);
                if(finJugada != 1 && finJugada != 2){
                    printf("Dato invalido, ingresa otra opcion\n");
                }
            }
            if(finJugada == 2){
                indices[i][0] = -1;
                return sumaIndices;
            }
        }
    }
    for(int k = 0; k < j;k++)
        sumaIndices += indices[i][k];
    indices[i][j] = -1;
    
    for(int j = 0; j < i; j++){
        for(int k = 0; indices[j][k] != -1; k++){
            posibleJugada[k] = actual->mano[indices[j][k]];
            arrSize = k;            
        }
        jugadaValida = revisarJugada(posibleJugada, arrSize);
        if(!jugadaValida){
            
        }
    }
}

void jugadaInicial(struct ColaJugadores *cola, struct Pila *pila, struct Pila *bote){
    int indices[8][MAX_JUGADA];//Cantidad de maximas jugadas posibles en una sola mano
    //Jugar la mano entera y descartar una
    //Jugar 4 juegos de 3
    //Jugar 3 juegos de 4
    //Jugar 2 juegos de 3 y 2 de 4 
    //8 en caso extremo y si el usuario tiene toda la baraja (poco probable)
    int sumaIndices = 0, finJugada = -1, i = 0, jugadaValida;
    while(sumaIndices<25 && finJugada != 2){
        ClearPlayerTurn();
        sumaIndices = iniciarJugada(cola->frente, indices, i);
        printf("La suma de tus fichas en juego es de: %d", sumaIndices);
        while(finJugada != 1 && finJugada != 2){
            printf("Deseas realizar otra jugada?\n");
            printf("1....Si\n");
            printf("2....No\n");
            scanf("%d", &finJugada);
            if(finJugada != 1 && finJugada != 2)
                printf("dato invalido, intenta de nuevo");
        }
        if(finJugada == 2 && sumaIndices < 25){
            while(finJugada != 1 && finJugada != 2){
            printf("Tus puntos no son suficientes, estas seguro de salir?\n");
            printf("1....No\n");
            printf("2....Si\n");
            scanf("%d", &finJugada);
            if(finJugada != 1 && finJugada != 2)
                printf("dato invalido, intenta de nuevo");
            }
            if(finJugada == 2){
                i = 0;
                comer(cola, pila, bote);
                break;
            }
        }
        if(finJugada == 1)
            i++;
    }
}
void finTurno(struct ColaJugadores *cola) {
    if (cola == NULL || cola->frente == NULL) {
        printf("Fin de la partida!\n");
        PCTurn(2);
        return;
    }
    // Avanzar el frente de la cola
    cola->frente = cola->frente->siguiente;
    cola->trasero = cola->trasero->siguiente;
}

void revisarSalida(struct ColaJugadores* cola, struct ColaJugadores* colaResultados, int *jugadoresActuales) {
    if (cola == NULL || cola->frente == NULL) {
        printf("Error: La cola de jugadores está vacía.\n");
        return;
    }

    struct Jugador* jugadorActual = cola->frente;

    // Verificar si el jugador no tiene fichas
    if (jugadorActual->numCartas == 0 || cola->frente == cola->trasero) {
        // Si el jugador no tiene fichas, sacarlo de la cola actual
        if (cola->frente == cola->trasero) {
            // Si era el único jugador en la cola, se vacía la cola
            cola->frente = NULL;
            cola->trasero = NULL;
        } else {
            cola->frente = jugadorActual->siguiente; // Avanzar el frente
            cola->trasero->siguiente = cola->frente; // Mantener circularidad
        }

        // Ahora agregamos el jugador a la cola de resultados
        if (colaResultados->frente == NULL) {
            // Si la cola de resultados está vacía, el jugador se convierte en el frente y el trasero
            colaResultados->frente = jugadorActual;
            colaResultados->trasero = jugadorActual;
        } else {
            // De lo contrario, agregamos al final de la cola de resultados
            colaResultados->trasero->siguiente = jugadorActual;
            colaResultados->trasero = jugadorActual; // Actualizar el trasero
        }

        jugadorActual->siguiente = NULL; // Desconectar para evitar circularidad en resultados
        if ((*jugadoresActuales) > 1)
            printf("%s ha salido del juego.\n", jugadorActual->nombre);
        else
            printf("El jugador %s ha salido porque ya no hay mas jugadores!.\n", jugadorActual->nombre);
        (*jugadoresActuales)--;
        PCTurn(2);
    }
}

void Leaderboard(struct ColaJugadores* colaResultados, int totalJugadores) {
    if (colaResultados == NULL || colaResultados->frente == NULL) {
        printf("No hay jugadores en la cola de resultados.\n");
        return;
    }

    struct Jugador* actual = colaResultados->frente;
    int posicion = 1; // Inicia la posición desde 1

    printf("Clasificacion del juego:\n");

    // Recorre la cola de resultados imprimiendo los jugadores en orden
    do {
        printf("%d.- %s\n", posicion, actual->nombre);

        if (actual->siguiente == NULL) {
            break; // Si hemos llegado al final de la cola de resultados, salimos del bucle
        }

        actual = actual->siguiente; // Avanza al siguiente jugador
        posicion++; // Aumenta la posición
    } while (actual != NULL && posicion <= totalJugadores);
}