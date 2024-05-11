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
#define MAX_ROWS 8
#define MAX_COLS 13
// Funciones generales
int randomNumber();
void colorReset();
void ClearPlayerTurn();
void PCTurn(int i);
// Funciones para inicio de juego
void createInitialDeck(struct Fichas Baraja[4][26]);
void createJokers(struct Fichas Comodin[2]);
void inicializarCola(struct ColaJugadores *cola);
void inicializarPila(struct Pila *pila);
void inicializarTablero(struct Tablero *tablero);
void inicializarJugada(struct Jugada *jugada);
void insertarJugador(struct ColaJugadores *cola, char nombre[], bool esBot);
void mezclarJugadores(struct ColaJugadores *cola, int totalJugadores);
void repartirCartasYPila(struct ColaJugadores *cola, struct Fichas baraja[4][26],
                         struct Fichas comodin[2], int totalJugadores, struct Pila *pila);
void revolver(struct Fichas *array, int n);
// Funciones para el juego
void imprimirJugada(struct Jugada *jugadaActual);
void imprimirTablero(struct Tablero *tablero);
void imprimirManos(struct ColaJugadores *cola, int totalJugadores);
void imprimirManoActual(struct Jugador *actual);
void imprimirIndices(struct Jugador *actual);
void ordenarMano(struct Fichas fichas[TAM_MAX], int n);
bool isJoker(int Joker);
void comer(struct ColaJugadores *cola, struct Pila *pila);
// Funciones para el tablero
void agregarJugada(struct Tablero *tablero, struct Jugada *nuevaJugada);
// Agregar fichas a juego existente
void agregarFichaPorDerecha(struct Jugada *jugada, struct Fichas valor);
void agregarFichaPorIzquierda(struct Jugada *jugada, struct Fichas valor);
// Robar fichas a juego existente
struct Fichas robarPorIzquierda(struct Jugada *jugada);
struct Fichas robarPorDerecha(struct Jugada *jugada);
// Funciones para el jugador
int revisarJugada(struct Fichas fichas[MAX_COLS], int arrSize, bool esBot);
int jugadaInicial(struct Tablero *tablero, struct ColaJugadores *cola, struct Pila *pila);
int iniciarJugada(struct Jugador *actual, int indices[MAX_COLS]);
// Funciones de control de juego
void finTurno(struct ColaJugadores *cola);
void revisarSalida(struct ColaJugadores *cola, struct ColaJugadores *colaResultados, int *jugadoresActuales);
// Funciones de final de juego
void Leaderboard(struct ColaJugadores *colaResultados, int totalJugadores);
#endif
