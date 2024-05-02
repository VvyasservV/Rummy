#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Functions.h"
#include "Structs.h"

int main() {
    int random;
    colorReset();
    int jugadoresActuales, totalJugadores = 0, jugadoresConsola = -1, bots = 0, opcion = 0;

    // Pedir al usuario el número total de jugadores
    while (totalJugadores < 2 || totalJugadores > 4) {
        printf("Ingrese el numero total de jugadores (entre 2 y 4): ");
        scanf("%d", &totalJugadores);
    }
    jugadoresActuales = totalJugadores;
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
    struct ColaJugadores cola, colaResultados;
    struct Pila pila, bote;

    // Crear la baraja y los comodines
    createInitialDeck(Baraja);
    createJokers(Comodin);

    // Inicializar la cola de jugadores
    inicializarCola(&cola);
    inicializarCola(&colaResultados);
    inicializarPila(&pila);
    inicializarPila(&bote);
    // Insertar jugadores en la cola
    for (int i = 0; i < jugadoresConsola; i++) {
        char nombre[50];
        printf("Ingrese el nombre del jugador %d: ", i+1);
        scanf("%49s", nombre);
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
    while (cola.frente != NULL){
        if(jugadoresConsola == 0)
            PCTurn(1);
        else
            ClearPlayerTurn();
        random  = randomNumber();
        colorReset();
        // Imprimir las manos de los jugadores
        imprimirManos(&cola, jugadoresActuales);
        colorReset();
        if(pila.top != 0)
            printf("POZO: %d\n", pila.top);
        else
            printf("POZO VACIO!\n");
        colorReset();
        if(bote.top != 0)
            if(isJoker(bote.array[bote.top-1].numero)) printf("POZO DE DESCARTES: %sJ\n", bote.array[bote.top-1].color);
            else printf("POZO DE DESCARTES: %s%d\n", bote.array[bote.top-1].color, bote.array[bote.top-1].numero);
        else
            printf("POZO DE DESCARTES VACIO!\n");
        colorReset();
        printf("Mesa:\n");
        if(cola.frente->esBot==0){
            //Logica del user
            if(cola.frente->jugadorActivo==0){
                //Primera tirada
                while(opcion != 1 && opcion != 2){
                    printf("\n1....Iniciar jugada\n");
                    printf("2....Comer y pasar\n");
                    printf("3...Ordenar fichas\n");
                    scanf("%d", &opcion);
                    if(opcion != 1 && opcion != 2 && opcion != 3){
                        printf("Opcion incorrecta, intentalo de nuevo\n");
                        scanf("%d", &opcion);
                        PCTurn(2);
                    }
                    if(opcion == 3){
                        ordenarMano(&cola.frente->mano, cola.frente->numCartas);             
                        printf("Mano ordenada\n");
                        imprimirManoActual(cola.frente);
                        colorReset();
                    }
                }
                if(opcion == 1){
                    ClearPlayerTurn();
                    jugadaInicial(&cola, &pila, &bote);
                }else if(opcion == 2){
                    comer(&cola, &pila, &bote);
                }
                descartar(&cola, &bote);
                finTurno(&cola);
            }else{
                //Segunda tirada, todas las funciones desbloqueadas
                while(opcion != 1 && opcion != 2){
                    printf("1....Iniciar jugada\n");
                    printf("2....Comer y pasar\n");
                    printf("3...Ordenar fichas\n");
                    scanf("%d", &opcion);
                    if(opcion != 1 && opcion != 2 && opcion != 3){
                        printf("Opcion incorrecta, intentalo de nuevo\n");
                        scanf("%d", &opcion);
                        PCTurn(2);
                    }
                    if(opcion == 3){
                        ordenarMano(&cola.frente->mano, &cola.frente->numCartas);
                    }
                }
                if(opcion == 1){
                    ClearPlayerTurn();
                }else if(opcion == 2){
                    comer(&cola, &pila, &bote);
                }
                descartar(&cola, &bote);
                finTurno(&cola);
            }
        }else{
            //Logica del bot
            comer(&cola, &pila, &bote);
            descartar(&cola, &bote);
            finTurno(&cola);
            if(cola.frente->jugadorActivo==0){
                //Primera tirada
                //jugadaInicial();
            }else{
                //Segunda tirada, todas las funciones desbloqueadas
            }
        }
        /*descartar(&cola, &bote);
        if(random==0)
            comer(&cola, &pila, &bote);
        */
        revisarSalida(&cola, &colaResultados, &jugadoresActuales);
        opcion=0;
    }
    Leaderboard(&colaResultados, totalJugadores);
    
    return 0;
}
