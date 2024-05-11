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
/*
------------------Indice-----------------------
Funciones generales.........................23
Funciones de inicio del juego...............48
Funciones para el juego.....................204
    Funciones para el tablero...............345
    Modificar jugadas en tablero............---
        Agregar fichas a juego existente....363
        Robar fichas a juego existente......399
    Funciones de jugador....................446
    Funciones de control de juego...........572
Funciones de final de juego.................624
*/

// Funciones generales
int randomNumber()
{
    srand(time(NULL));
    int random = rand() % 5;
    return random;
}
void colorReset()
{
    printf(BLANCO "");
}
void ClearPlayerTurn()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void PCTurn(int i)
{
#ifdef _WIN32
    Sleep(i * 1000);
    system("cls");
#else
    sleep(i * 1000);
    system("clear");
#endif
}
// Funciones de inicio del juego
void createInitialDeck(struct Fichas Baraja[4][26])
{
    srand(time(NULL));
    int i;
    for (i = 0; i < 26; i++)
    {
        Baraja[0][i].numero = (i % 13) + 1;
        Baraja[0][i].color = VERDE;
        Baraja[0][i].asignada = false;
    }
    for (i = 0; i < 26; i++)
    {
        Baraja[1][i].numero = (i % 13) + 1;
        Baraja[1][i].color = AZUL;
        Baraja[1][i].asignada = false;
    }
    for (i = 0; i < 26; i++)
    {
        Baraja[2][i].numero = (i % 13) + 1;
        Baraja[2][i].color = NARANJA;
        Baraja[2][i].asignada = false;
    }
    for (i = 0; i < 26; i++)
    {
        Baraja[3][i].numero = (i % 13) + 1;
        Baraja[3][i].color = ROSA;
        Baraja[3][i].asignada = false;
    }
}
void createJokers(struct Fichas Comodin[2])
{
    int i;
    for (i = 0; i < 2; i++)
    {
        Comodin[i].numero = 99;
        Comodin[i].color = BLANCO;
        Comodin[i].asignada = false;
    }
}
void inicializarCola(struct ColaJugadores *cola)
{
    cola->frente = NULL;
    cola->trasero = NULL;
}
void inicializarPila(struct Pila *pila)
{
    pila->top = 0; // Inicializar el top de la pila en 0
}
void inicializarTablero(struct Tablero *tablero) {
    tablero->cabeza = NULL;
    tablero->tamanio = 0;
}
void inicializarJugada(struct Jugada *jugada) {
    jugada->cabeza = NULL;
    jugada->tamanio = 0;
}
void insertarJugador(struct ColaJugadores *cola, char nombre[], bool esBot)
{
    struct Jugador *nuevoJugador = (struct Jugador *)malloc(sizeof(struct Jugador));
    if (esBot)
    {
        static int numBot = 1;
        sprintf(nuevoJugador->nombre, "Bot %d", numBot++);
    }
    else
    {
        strcpy(nuevoJugador->nombre, nombre);
    }
    nuevoJugador->esBot = esBot;
    nuevoJugador->siguiente = NULL;
    nuevoJugador->numCartas = 0;
    nuevoJugador->jugadorActivo = false;
    if (cola->frente == NULL)
    {
        cola->frente = nuevoJugador;
    }
    else
    {
        cola->trasero->siguiente = nuevoJugador;
    }

    cola->trasero = nuevoJugador;

    cola->trasero->siguiente = cola->frente;
}
void mezclarJugadores(struct ColaJugadores *cola, int totalJugadores)
{
    if (totalJugadores <= 0)
    {
        printf("Error: totalJugadores es 0 o negativo.\n");
        return; // Si no hay jugadores, no se hace nada
    }

    if (cola->frente == NULL)
    {
        printf("Error: la cola esta vacia.\n");
        return; // Si la cola esta vacia, no hay nada que mezclar
    }

    struct Jugador *jugadores[MAX_JUGADORES];
    struct Jugador *actual = cola->frente;

    // Cargar todos los jugadores en un arreglo
    for (int i = 0; i < totalJugadores; i++)
    {
        jugadores[i] = actual;
        actual = actual->siguiente;
    }

    for (int i = totalJugadores - 1; i > 0; i--)
    {
        int j = rand() % (i + 1); // indice aleatorio
        // Intercambiar las posiciones i y j
        struct Jugador *temp = jugadores[i];
        jugadores[i] = jugadores[j];
        jugadores[j] = temp;
    }

    // Reconectar la cola para mantener la circularidad
    for (int i = 0; i < totalJugadores - 1; i++)
    {
        jugadores[i]->siguiente = jugadores[i + 1];
    }
    jugadores[totalJugadores - 1]->siguiente = jugadores[0]; // Circularidad
    cola->frente = jugadores[0];                             // Nuevo frente
    cola->trasero = jugadores[totalJugadores - 1];           // Nuevo trasero

    for (int i = 0; i < totalJugadores; i++)
    {
        printf("Jugador %d: %s\n", i + 1, jugadores[i]->nombre);
    }
}
void repartirCartasYPila(struct ColaJugadores *cola, struct Fichas baraja[4][26],
                         struct Fichas comodin[2], int totalJugadores, struct Pila *pila)
{
    struct Jugador *actual = cola->frente;
    int cartaPila = 0;
    int totalCartas = 4 * 26 + 2;    // Total de cartas en la baraja, incluyendo los comodines
    int totalManos = totalJugadores; // Total de manos que se repartiran

    // Crear un arreglo temporal para barajar las cartas
    struct Fichas tempBaraja[4 * 26 + 2];
    int idx = 0, k = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            tempBaraja[idx++] = baraja[i][j];
        }
    }
    // Agregar los comodines al arreglo temporal
    tempBaraja[idx++] = comodin[0];
    tempBaraja[idx++] = comodin[1];

    // Barajar el arreglo de cartas
    revolver(tempBaraja, totalCartas);

    // Repartir las cartas aleatorias a cada jugador
    for (int i = 0; i < totalManos; i++)
    {
        for (int j = 0; j < 14; j++)
        { // Cada jugador recibe 14 cartas
            // Verificar si aun quedan cartas en el arreglo barajado
            if (k < totalCartas)
            {
                actual->mano[j] = tempBaraja[k++];
            }
            else
            {
                // Si no quedan mas cartas, salir del bucle
                break;
            }
            actual->numCartas++;
        }
        // Avanzar al siguiente jugador despues de repartir todas las cartas de la mano actual
        actual = actual->siguiente;
    }

    // Llenar la pila con las cartas restantes
    for (; k < totalCartas; k++)
    {
        pila->array[cartaPila++] = tempBaraja[k];
        pila->top++; // Actualizar el valor de top
    }
}
void revolver(struct Fichas *array, int n)
{
    int i;
    for (i = 0; i < n - 1; i++)
    {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        struct Fichas temp = array[j];
        array[j] = array[i];
        array[i] = temp;
    }
}

// Funciones para el juego
void imprimirTablero(struct Tablero *tablero)
{
    if (tablero->cabeza == NULL)
    {
        printf("El tablero esta vacio.\n");
        return;
    }

    struct NodoTablero *jugadaActual = tablero->cabeza;
    int jugadaNumero = 1;

    printf("--Jugadas en el tablero--\n");
    do
    {
        printf("Jugada %d:\n", jugadaNumero);
        struct Nodo *fichaActual = jugadaActual->lista->cabeza;

        do
        {
            printf("%s%d ", fichaActual->ficha.color, fichaActual->ficha.numero);
            fichaActual = fichaActual->siguiente;

            // Impresiones para depurar
            printf("fichaActual: %p, listaCabeza: %p\n", (void *)fichaActual, (void *)jugadaActual->lista->cabeza);

        } while (fichaActual != jugadaActual->lista->cabeza);

        printf("\n");  // Nueva línea para separar las jugadas
        jugadaActual = jugadaActual->siguiente;
        jugadaNumero++;

    } while (jugadaActual != tablero->cabeza);
}


void imprimirManos(struct ColaJugadores *cola, int totalJugadores)
{
    struct Jugador *actual = cola->frente;

    // Imprimir las manos de los jugadores reales
    for (int i = 1; i <= totalJugadores; i++)
    {
        colorReset();
        printf("%s: \nFichas:\t", actual->nombre);
        if (actual == cola->frente)
        {
            imprimirManoActual(actual);
        }
        else
        {
            for (int j = 0; j < actual->numCartas; j++)
            {
                printf("%s*\t", GRIS);
                if (j != 0 && j % 15 == 0)
                    printf("\n");
            }
        }
        printf("\n");
        actual = actual->siguiente;
    }
}
void imprimirManoActual(struct Jugador *actual)
{
    for (int j = 0; j < actual->numCartas; j++)
    {
        if (isJoker(actual->mano[j].numero))
            printf("%sJ\t", actual->mano[j].color);
        else
            printf("%s%d\t", actual->mano[j].color, actual->mano[j].numero);
    }
}
void imprimirIndices(struct Jugador *actual)
{
    printf("%s", BLANCO);
    for (int j = 0; j < actual->numCartas; j++)
    {
        printf("%d\t", (j + 1));
    }
}
void ordenarMano(struct Fichas fichas[TAM_MAX], int n)
{
    if (n <= 1)
    {
        return; // No hay nada que ordenar si solo hay una carta o ninguna
    }

    for (int i = 1; i < n; i++)
    {
        struct Fichas key = fichas[i]; // Guarda la ficha actual
        int j = i - 1;
        // Compara primero por numero y luego por color
        while (j >= 0 && (fichas[j].numero > key.numero || 
            (fichas[j].numero == key.numero && strcmp(fichas[j].color, key.color) > 0)))
        {
            fichas[j + 1] = fichas[j]; // Desplaza la ficha hacia la derecha
            j--;                       // Mueve el indice hacia la izquierda
        }
        // Inserta la ficha en la posicion correcta
        fichas[j + 1] = key;
    }
}

bool isJoker(int Joker)
{
    if (Joker == 99)
        return 1;
    return 0;
}
void comer(struct ColaJugadores *cola, struct Pila *pila)
{
    struct Jugador *actual = cola->frente;
    if (pila->top > 0)
    {
        pila->top--;
        actual->mano[actual->numCartas++] = pila->array[pila->top];
    }
    finTurno(cola);
}

// Funciones para el tablero
void agregarJugada(struct Tablero *tablero, struct Jugada *nueva_lista)
{
    struct NodoTablero *nuevo_nodo = (struct NodoTablero *)malloc(sizeof(struct NodoTablero));
    nuevo_nodo->lista = nueva_lista;

    if (tablero->cabeza == NULL)
    {
        tablero->cabeza = nuevo_nodo;
        nuevo_nodo->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = nuevo_nodo;
    }
    else
    {
        struct NodoTablero *ultimo = tablero->cabeza->anterior;
        ultimo->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = ultimo;
        nuevo_nodo->siguiente = tablero->cabeza;
        tablero->cabeza->anterior = nuevo_nodo;
    }
    tablero->tamanio++;

    // Imprimir la jugada en el tablero
    printf("Jugada %d agregada:\n", tablero->tamanio);
    struct Nodo *actual = nueva_lista->cabeza;
    do
    {
        printf("%s%d ", actual->ficha.color, actual->ficha.numero);
        actual = actual->siguiente;
    }while (actual != nueva_lista->cabeza);
    printf("%s\n", BLANCO); // Imprimir el color de la ficha de la jugada
}

// Agregar fichas a juego existente
void agregarFichaPorDerecha(struct Jugada *jugada, struct Fichas valor)
{
    struct Nodo *nuevo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevo_nodo->ficha = valor;
    printf("%s%d %s agregada\n", nuevo_nodo->ficha.color, nuevo_nodo->ficha.numero, BLANCO);
    if (jugada->cabeza == NULL)
    {
        jugada->cabeza = nuevo_nodo;
        jugada->cabeza->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
    }
    else
    {
        struct Nodo *cola = jugada->cabeza->anterior;
        cola->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = cola;
        nuevo_nodo->siguiente = jugada->cabeza;
        jugada->cabeza->anterior = nuevo_nodo;
    }

    jugada->tamanio++;
}
void agregarFichaPorIzquierda(struct Jugada *jugada, struct Fichas valor)
{
    struct Nodo *nuevo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevo_nodo->ficha = valor;

    if (jugada->cabeza == NULL)
    {
        jugada->cabeza = nuevo_nodo;
        jugada->cabeza->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
    }
    else
    {
        struct Nodo *cola = jugada->cabeza->anterior;
        nuevo_nodo->siguiente = jugada->cabeza;
        nuevo_nodo->anterior = cola;
        cola->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
        jugada->cabeza = nuevo_nodo;
    }
    jugada->tamanio++;
}
// Robar fichas a juego existente
struct Fichas robarPorIzquierda(struct Jugada *jugada)
{
    if (jugada->cabeza == NULL)
    {
        printf("Lista vacia, no se puede robar por inicio.\n");
        return (struct Fichas){-1, "N/A", false}; // Indicador de error
    }

    struct Nodo *robar_nodo = jugada->cabeza;
    struct Fichas resultado = robar_nodo->ficha;

    if (jugada->tamanio == 1)
    {
        jugada->cabeza = NULL;
    }
    else
    {
        jugada->cabeza = robar_nodo->siguiente;
        jugada->cabeza->anterior = robar_nodo->anterior;
        robar_nodo->anterior->siguiente = jugada->cabeza;
    }

    free(robar_nodo);
    jugada->tamanio--;

    return resultado;
}
struct Fichas robarPorDerecha(struct Jugada *jugada)
{
    if (jugada->cabeza == NULL)
    {
        printf("Lista vacia, no se puede robar por final.\n");
        return (struct Fichas){-1, "N/A", false}; // Indicador de error
    }

    struct Nodo *ultimo = jugada->cabeza->anterior;
    struct Fichas resultado = ultimo->ficha;

    if (jugada->tamanio == 1)
    {
        jugada->cabeza = NULL;
    }
    else
    {
        ultimo->anterior->siguiente = jugada->cabeza;
        jugada->cabeza->anterior = ultimo->anterior;
    }

    free(ultimo);
    jugada->tamanio--;

    return resultado;
}

void imprimirIndicesJugadas(int ListSize)
{
    printf("%s0", BLANCO);
    for (int j = 0; j < ListSize - 1; j++)
    {
        printf("\t ");
    }
    printf("1\n");
}

// Funciones para el jugador
// Funciones jugadorActivo = 0 (Jugador no ha sumado 25 puntos)
bool detectarDuplicados(int indices[MAX_ROWS][MAX_COLS], int totalFilas, int columnas)
{
    for (int i = 0; i < totalFilas - 1; i++)
    {
        if (indices[i][0] == -1)
        {
            continue; // Saltar a la siguiente fila si la fila actual comienza con -1
        }
        for (int j = i + 1; j < totalFilas; j++)
        {
            if (indices[j][0] == -1)
            {
                continue; // Saltar a la siguiente fila si la fila comparada comienza con -1
            }
            // Calcular el número de elementos válidos en ambas filas
            int elementosValidosI = 0, elementosValidosJ = 0;
            for (int k = 0; k < columnas; k++)
            {
                if (indices[i][k] != -1)
                {
                    elementosValidosI++;
                }
                if (indices[j][k] != -1)
                {
                    elementosValidosJ++;
                }
            }
            // Comparar solo los elementos válidos en ambas filas
            for (int k = 0; k < elementosValidosI && k < elementosValidosJ; k++)
            {
                if (indices[i][k] == indices[j][k])
                {
                    return true; // Se encontró un duplicado
                }
            }
        }
    }
    return false; // No se encontraron duplicados
}

// Funcion para detectar duplicados en una fila

// Funcion para resolver duplicados en una fila y decidir si eliminarla
int encontrarUltimaFila(int indices[MAX_ROWS][MAX_COLS]) {
    int ultimaFila = -1;
    for (int i = 0; i < MAX_ROWS; i++) {
        if (indices[i][0] != -1) {
            ultimaFila = i;
        }
    }
    return ultimaFila;
}

int resolverDuplicados(int indices[MAX_ROWS][MAX_COLS], int fila, int columnas, struct Fichas *mano) {
    int ultimaFila = encontrarUltimaFila(indices);

    if (ultimaFila == -1) {
        printf("No hay filas existentes para comparar.\n");
        return -1; // No hay filas existentes para comparar
    }

    if (detectarDuplicados(indices, ultimaFila, columnas)) {
        printf("Se encontraron duplicados en la fila %d con la última fila existente.\n", fila);
        printf("Cartas duplicadas: ");
        for (int i = 0; i < columnas; i++) {
            if (indices[ultimaFila][i] != -1) {
                printf("%s%d%s", mano[indices[ultimaFila][i]].color, mano[indices[ultimaFila][i]].numero, (i < columnas - 1) ? ", " : "\n");
            }
        }

        printf("Elija la fila a conservar:\n");
        printf("1... Mantener la última fila existente\n");
        printf("2... Eliminar duplicados y conservar fila %d\n", fila);

        int eleccion;
        scanf("%d", &eleccion);

        if (eleccion == 1) {
            // Eliminar duplicados de la fila seleccionada
            for (int i = 0; i < columnas; i++) {
                if (indices[fila][i] == -1) {
                    continue; // Ignorar elementos inválidos
                }
                for (int j = i + 1; j < columnas; j++) {
                    if (indices[fila][j] == indices[fila][i]) {
                        indices[fila][j] = -1; // Eliminar duplicados
                    }
                }
            }
            return -2; // Retorna -2 para indicar que se mantuvo la fila actual
        } else if (eleccion == 2) {
            // Sumar las cartas de la fila que se va a eliminar
            int sumaCartas = 0;
            for (int i = 0; i < columnas; i++) {
                if (indices[fila][i] != -1) {
                    sumaCartas += mano[indices[fila][i]].numero;
                }
            }
            // Marcar la fila para eliminarla
            for (int i = 0; i < MAX_ROWS; i++) {
                if (indices[i][0] == fila) {
                    for (int j = 0; j < MAX_COLS; j++) {
                        indices[i][j] = -1; // Rellenar la fila con -1 para indicar que está eliminada
                    }
                    return sumaCartas; // Retorna la suma de las cartas de la fila eliminada
                }
            }
        } else {
            printf("Opción no válida. No se realizó ninguna acción.\n");
            return -1; // Retorna -1 si no se realizó ninguna acción válida
        }
    }
    return -1; // Retorna -1 si no se encontraron duplicados
}


void ajustar_indices(int matriz[MAX_ROWS][MAX_COLS], int fila_eliminada[], int fila_size, int total_rows)
{
    // Crear un array de flags para marcar indices eliminados
    bool eliminado[MAX_COLS] = {false};

    // Marcar indices eliminados
    for (int i = 0; i < fila_size; i++)
    {
        int index = fila_eliminada[i];
        if (index >= 0 && index < MAX_COLS)
        {
            eliminado[index] = true; // Marcar como eliminado
        }
    }

    // Ajustar filas posteriores segun los indices eliminados
    for (int row = 0; row < total_rows; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            int original = matriz[row][col];
            if (original == -1)
            {
                continue; // Ignorar elementos ya eliminados
            }

            int offset = 0;

            // Calcular offset segun cuantos indices eliminados estan antes del indice actual
            for (int i = 0; i < original; i++)
            {
                if (eliminado[i])
                {
                    offset++; // Incrementa el offset para cada indice eliminado
                }
            }

            // Ajustar el indice restando el offset
            int ajustado = original - offset;

            if (ajustado < 0)
            { // Si el resultado es negativo, significa que se elimino
                matriz[row][col] = -1;
            }
            else
            {
                matriz[row][col] = ajustado;
            }
        }
    }
}

void jugadaInicial(struct Tablero *tablero, struct ColaJugadores *cola, struct Pila *pila)
{
    int indices[MAX_ROWS][MAX_COLS];
    struct Jugada nuevaJugada;
    inicializarJugada(&nuevaJugada);
    struct Jugador actual = *cola->frente;
    for (int i = 0; i < MAX_ROWS; i++){
        for(int j = 0; j < MAX_COLS; j++)
            indices[i][j] = -1;
    }
    int sumaCartas = 0, finJugada = 0, i = 0, c, jugadaValida, k = 0;
    while (sumaCartas < 25 || finJugada != 2)
    {
        c = 0;
        ClearPlayerTurn();
        int sumaActual = iniciarJugada(&actual, indices[i]); // Calcular la suma de la jugada actual
        while (indices[i][c] != -1)
            c++;
        resolverDuplicados(indices, i, c, actual.mano); // Resolver duplicados despues de iniciar jugada

        // Verificar si hay duplicados y si se debe mantener la fila anterior o la nueva
        if (indices[i][0] != -1)
        {
            sumaCartas += sumaActual; // Sumar la nueva jugada solo si no hay duplicados o si se elige la nueva jugada
            i++;
        }

        printf("La suma de tus fichas en juego es de: %d\n", sumaCartas);

        // Opciones para agregar mas jugadas
        while (finJugada != 1 && finJugada != 2)
        {
            printf("Deseas realizar otra jugada?\n");
            printf("1....Si\n");
            printf("2....No\n");
            scanf("%d", &finJugada);
            if (finJugada != 1 && finJugada != 2)
            {
                printf("Opcion no valida, intenta de nuevo");
            }
        }

        if (finJugada == 2 && sumaCartas < 25)
        {
            printf("Suma insuficiente. ¿Deseas salir?\n");
            printf("1....No\n");
            printf("2....Si\n");
            scanf("%d", &finJugada);
            if (finJugada == 2)
            {
                comer(cola, pila);
                break;
            }
        }
    }

    if (sumaCartas >= 25)
    {
        // Procesar las jugadas
        printf("Procesando jugada\n");
        for (int j = 0; j < i; j++)
        {
            if (indices[j][0] != -1)
            { // Solo procesar filas validas
                k = 0;
                while (indices[j][k] != -1)
                {
                    agregarFichaPorDerecha(&nuevaJugada, actual.mano[indices[j][k]]);
                    k++;
                }
                printf("Jugada correctamente agregada\n");
                // Eliminar las fichas utilizadas
                while (k > 0)
                {
                    int pos = indices[j][--k];
                    for (int h = pos; h < actual.numCartas - 1; h++)
                    {
                        actual.mano[h] = actual.mano[h + 1];
                    }
                    actual.numCartas--;
                }
                imprimirManoActual(&actual);
                agregarJugada(tablero, &nuevaJugada);
                finTurno(cola);
            }
        }
        actual.jugadorActivo = 1; // El jugador ya hizo su jugada inicial

    }
}

int iniciarJugada(struct Jugador *actual, int *arrIndices)
{
    int finJugada = 0, indiceSeleccionado = 0, arrSize = 0, sumaCartas = 0, jugadaValida = 0;
    struct Fichas posibleJugada[MAX_COLS]; // Solo se usa para verificar jugada

    while (finJugada != 2 && arrSize < MAX_COLS)
    {
        printf("Selecciona los indices de las cartas a jugar\n");
        imprimirManoActual(actual);
        printf("\n");
        imprimirIndices(actual);
        printf("\n");

        do
        {
            if (scanf("%d", &indiceSeleccionado) != 1)
            {
                printf("Error en la entrada. Intenta de nuevo.\n");
                // Limpiar el buffer de entrada para evitar problemas con scanf
                while (getchar() != '\n')
                    ;
                continue;
            }
            if (indiceSeleccionado < 1 || indiceSeleccionado > actual->numCartas)
            {
                printf("Dato invalido, ingresa otro indice entre 1 y %d\n", actual->numCartas);
            }
        } while (indiceSeleccionado < 1 || indiceSeleccionado > actual->numCartas);

        arrIndices[arrSize] = indiceSeleccionado - 1;
        arrSize++;

        do
        {
            printf("Deseas agregar otro indice?\n");
            printf("1...Si\n");
            printf("2...No\n");
            if (scanf("%d", &finJugada) != 1 || (finJugada < 1 || finJugada > 2))
            {
                printf("Opcion no valida. Intenta de nuevo.\n");
                // Limpiar el buffer para evitar problemas con scanf
                while (getchar() != '\n')
                    ;
                finJugada = 0; // Para forzar a seguir en el bucle
            }
        } while (finJugada < 1 || finJugada > 2);

        if (finJugada == 2 && arrSize < 3)
        {
            printf("Faltan fichas para crear un juego valido\n");
            printf("¿Estas seguro de querer salir?\n");
            printf("1...No\n");
            printf("2...Si\n");
            if (scanf("%d", &finJugada) != 1 || (finJugada < 1 || finJugada > 2))
            {
                printf("Opcion no valida. Intenta de nuevo.\n");
                // Limpiar el buffer de entrada para evitar problemas
                while (getchar() != '\n')
                    ;
                finJugada = 0; // Para reiniciar el bucle
            }
        }

        if (finJugada == 2 && arrIndices[0] == -1)
        {
            return 0;
        }
    }
    for (int k = 0; k < arrSize; k++)
    {
        posibleJugada[k] = actual->mano[arrIndices[k]];
        printf("%s%d \t", posibleJugada[k].color, posibleJugada[k].numero);
    }
    printf(BLANCO "\n");
    int tipoJugada = revisarJugada(posibleJugada, arrSize, actual->esBot);

    if (tipoJugada == 0)
    {
        return 0; // Jugada invalida
    }
    for (int k = 0; k < arrSize; k++)
    {
        if (!isJoker(actual->mano[arrIndices[k]].numero))
            sumaCartas += actual->mano[arrIndices[k]].numero;

        else
        {
            if (tipoJugada == 1)
            {
                // corrida
                int valorComodin = obtenerValorComodin(actual->mano, arrIndices, arrSize);
                sumaCartas += valorComodin;
            }
            else if (tipoJugada == 2)
            {
                if (k == 0)
                    sumaCartas += actual->mano[arrIndices[k + 1]].numero;
                else
                    sumaCartas += actual->mano[arrIndices[k - 1]].numero;
            }
        }
    }

    return sumaCartas;
}

int obtenerValorComodin(struct Fichas *mano, int *arrIndices, int arrSize)
{
    // Buscar el valor mas alto y mas bajo de las cartas seleccionadas
    int valorMasAlto = 0;
    int valorMasBajo = 99; // Valor alto inicial para garantizar que se actualice en la busqueda

    for (int i = 0; i < arrSize; i++)
    {
        int valorCarta = mano[arrIndices[i]].numero;
        if (valorCarta > valorMasAlto && valorCarta != 99)
        {
            valorMasAlto = valorCarta;
        }
        if (valorCarta < valorMasBajo)
        {
            valorMasBajo = valorCarta;
        }
    }

    // Si el comodin esta en una corrida, su valor es la carta mas alta + 1 si no excede el limite, de lo contrario, es la carta mas baja - 1
    if (valorMasAlto < 12)
    {
        return valorMasAlto + 1;
    }
    else
    {
        return valorMasBajo - 1;
    }
}

int revisarJugada(struct Fichas fichas[MAX_COLS], int arrSize, bool esBot)
{
    // Ordena el arreglo para verificar las corridas
    ordenarMano(fichas, arrSize);

    // Verifica corridas
    for (int i = 0; i < arrSize - 1; i++)
    {
        if ((fichas[i + 1].numero == fichas[i].numero + 1 &&
             strcmp(fichas[i + 1].color, fichas[i].color) == 0) ||
            (fichas[i + 1].numero == 99 && i == arrSize - 2))
        {             // Corrida encontrada
            return 1; // Corrida
        }
    }

    // Verifica "OAK" (Tres iguales)
    int OAK = 0; // Contador de cartas OAK
    if (arrSize >= 3 && arrSize <= 4)
    {
        for (int i = 0; i < arrSize - 1; i++)
        {
            if ((fichas[i + 1].numero == fichas[i].numero &&
                strcmp(fichas[i + 1].color, fichas[i].color) != 0) ||
                 (fichas[i + 1].numero == 99 && i == arrSize - 2))
            {
                OAK++;
            }
            else
            {
                OAK = 0; // Reiniciar contador si no hay una secuencia continua
            }
        }
    }
    if (OAK == arrSize - 1)
    {
        return 2; // OAK
    }

    // Si no se encontraron corridas ni OAK, la jugada es invalida
    if (!esBot)
    {
        printf("La jugada que ingresaste no es valida\n");
    }
    return 0; // Jugada invalida
}

// Funciones de control de juego
void finTurno(struct ColaJugadores *cola)
{
    if (cola == NULL || cola->frente == NULL)
    {
        printf("Fin de la partida!\n");
        PCTurn(2);
        return;
    }
    // Avanzar el frente de la cola
    cola->frente = cola->frente->siguiente;
    cola->trasero = cola->trasero->siguiente;
}
void revisarSalida(struct ColaJugadores *cola, struct ColaJugadores *colaResultados, int *jugadoresActuales)
{
    if (cola == NULL || cola->frente == NULL)
    {
        printf("Error: La cola de jugadores esta vacia.\n");
        return;
    }

    struct Jugador *jugadorActual = cola->frente;

    // Verificar si el jugador no tiene fichas
    if (jugadorActual->numCartas == 0 || cola->frente == cola->trasero)
    {
        // Si el jugador no tiene fichas, sacarlo de la cola actual
        if (cola->frente == cola->trasero)
        {
            // Si era el unico jugador en la cola, se vacia la cola
            cola->frente = NULL;
            cola->trasero = NULL;
        }
        else
        {
            cola->frente = jugadorActual->siguiente; // Avanzar el frente
            cola->trasero->siguiente = cola->frente; // Mantener circularidad
        }

        // Ahora agregamos el jugador a la cola de resultados
        if (colaResultados->frente == NULL)
        {
            // Si la cola de resultados esta vacia, el jugador se convierte en el frente y el trasero
            colaResultados->frente = jugadorActual;
            colaResultados->trasero = jugadorActual;
        }
        else
        {
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

// Funciones de final de juego
void Leaderboard(struct ColaJugadores *colaResultados, int totalJugadores)
{
    if (colaResultados == NULL || colaResultados->frente == NULL)
    {
        printf("No hay jugadores en la cola de resultados.\n");
        return;
    }

    struct Jugador *actual = colaResultados->frente;
    int posicion = 1; // Inicia la posicion desde 1

    printf("Clasificacion del juego:\n");

    // Recorre la cola de resultados imprimiendo los jugadores en orden
    do
    {
        printf("%d.- %s\n", posicion, actual->nombre);

        if (actual->siguiente == NULL)
        {
            break; // Si hemos llegado al final de la cola de resultados, salimos del bucle
        }

        actual = actual->siguiente; // Avanza al siguiente jugador
        posicion++;                 // Aumenta la posicion
    } while (actual != NULL && posicion <= totalJugadores);
}
