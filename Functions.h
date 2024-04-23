#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Structs.h"

#define AZUL "\x1B[38;2;61;99;216m"
#define VERDE "\x1B[38;2;46;249;77m"
#define ROSA "\x1B[38;2;243;39;249m"
#define NARANJA "\x1B[38;2;249;156;46m"
#define BLANCO "\x1B[37m"

// Funciones para limpiar la pantalla
void colorReset();
void ClearPlayerTurn();
void PCTurn(int i);

// Funciones para manipular la cola de jugadores
bool isJoker(int Joker);
void inicializarPila(struct Pila *pila);
void inicializarCola(struct ColaJugadores *cola);
void insertarJugador(struct ColaJugadores *cola, char nombre[], bool esBot);

void repartirCartasYPila(struct ColaJugadores *cola, struct Fichas baraja[4][26],
 struct Fichas comodin[2], int totalJugadores, struct Pila *pila);
// Funciones para crear la baraja y los comodines
void createInitialDeck(struct Fichas Baraja[4][26]);
void createJokers(struct Fichas Comodin[2]);

void imprimirManos(struct ColaJugadores *cola, int totalJugadores);
#endif
