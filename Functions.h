#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Structs.h"

#define AZUL "\x1B[38;2;61;99;216m"
#define VERDE "\x1B[38;2;46;249;77m"
#define ROSA "\x1B[38;2;243;39;249m"
#define NARANJA "\x1B[38;2;249;156;46m"
#define GRIS "\x1B[38;2;161;159;156m"
#define BLANCO "\x1B[37m"
#define MAX_JUGADORES 4
// Funciones para limpiar la pantalla
int randomNumber();
void colorReset();
void ClearPlayerTurn();
void PCTurn(int i);

// Funciones para manipular la cola de jugadores
bool isJoker(int Joker);
void createInitialDeck(struct Fichas Baraja[4][26]);
void createJokers(struct Fichas Comodin[2]);
void inicializarPila(struct Pila *pila);
void inicializarCola(struct ColaJugadores *cola);

void insertarJugador(struct ColaJugadores *cola, char nombre[], bool esBot);
void repartirCartasYPila(struct ColaJugadores *cola, struct Fichas baraja[4][26],
 struct Fichas comodin[2], int totalJugadores, struct Pila *pila);
void mezclarJugadores(struct ColaJugadores *cola, int totalJugadores);
void imprimirManos(struct ColaJugadores *cola, int totalJugadores);

void comer(struct ColaJugadores *cola, struct Pila *pila, struct Pila *bote);
void descartar(struct ColaJugadores *cola, struct Pila *bote);
void finTurno(struct ColaJugadores *cola);
void revisarSalida(struct ColaJugadores* cola, struct ColaJugadores* colaResultados, int *jugadoresActuales);
void Leaderboard(struct ColaJugadores* colaResultados, int totalJugadores);
#endif
