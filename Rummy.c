#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Functions.h"
#include "Structs.h"

int main() {
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
    mezclarJugadores(&cola, totalJugadores);
    // Repartir cartas a cada jugador y llenar la pila con el resto
    repartirCartasYPila(&cola, Baraja, Comodin, totalJugadores, &pila);

    // Imprimir la pila
    printf("\nCartas restantes en la pila:\n");
    for (int i = 0; i < pila.top; i++) {
        if(isJoker(pila.array[i].numero))
            printf("%s J   ", pila.array[i].color);
        else
            printf("%s %d   ", pila.array[i].color, pila.array[i].numero);
        if(i%13==0){
            printf("\n");
        }
    }
    colorReset();
    // Imprimir las manos de los jugadores
    imprimirManos(&cola, totalJugadores);
    colorReset();
    return 0;
}
