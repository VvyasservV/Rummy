#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Functions.h"
#include "Structs.h"

int main() {
    int random;
    colorReset();
    int totalJugadores = 0, jugadoresConsola = -1, bots = 0;

    // Pedir al usuario el número total de jugadores
    while (totalJugadores < 2 || totalJugadores > 4) {
        printf("Ingrese el numero total de jugadores (entre 2 y 4): ");
        scanf("%d", &totalJugadores);
    }

    // Pedir al usuario el número de jugadores en consola
    while (jugadoresConsola < 0 || jugadoresConsola > totalJugadores) {
        printf("Ingrese el numero de jugadores en consola (entre 0 y %d): ", totalJugadores);
        scanf("%d", &jugadoresConsola);
    }

    // Calcular el número de bots
    bots = totalJugadores - jugadoresConsola;

    // Crear las estructuras del juego
    struct Fichas Baraja[4][26];
    struct Fichas Comodin[2];
    struct ColaJugadores cola;
    struct Pila pila, bote;

    // Crear la baraja y los comodines
    createInitialDeck(Baraja);
    createJokers(Comodin);

    // Inicializar la cola de jugadores
    inicializarCola(&cola);
    inicializarPila(&pila);
    inicializarPila(&bote);
    // Insertar jugadores en la cola
    for (int i = 0; i < jugadoresConsola; i++) {
        char nombre[50];
        printf("Ingrese el nombre del jugador %d: ", i+1);
        scanf("%s", nombre);
        insertarJugador(&cola, nombre, false);
    }
    for (int i = 0; i < bots; i++) {
        char bot[50];
        sprintf(bot, "Bot %d", i+1);
        insertarJugador(&cola, bot, true);
    }
    ClearPlayerTurn();
    printf("Orden de los jugadores\n");
    mezclarJugadores(&cola, totalJugadores);
    PCTurn(5);
    // Repartir cartas a cada jugador y llenar la pila con el resto
    repartirCartasYPila(&cola, Baraja, Comodin, totalJugadores, &pila);

    // Imprimir la pila
    while (&cola != NULL){
        random  = randomNumber();
        colorReset();
        // Imprimir las manos de los jugadores
        imprimirManos(&cola, totalJugadores);
        colorReset();
        if(pila.top != 0)
            printf("POZO: %d\n", pila.top);
        else
            printf("POZO VACIO!\n");
        colorReset();
        if(bote.top != 0)
            printf("POZO DE DESCARTES: %s%d\n", bote.array[bote.top-1].color, bote.array[bote.top-1].numero);
        else
            printf("POZO DE DESCARTES VACIO!\n");
        colorReset();
        printf("Mesa:\n");
        PCTurn(1);
        comer(&cola, &pila, &bote);
        if(random==0)
            descartar(&cola, &bote);
        finTurno(&cola);
    }
    
    
    
    return 0;
}
