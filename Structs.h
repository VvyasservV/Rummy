#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdbool.h>
#include <stddef.h>
#define TAM_MAX 106

struct Fichas {
    int numero;
    char *color;
    bool asignada;
};
//Nodo Jugada
struct Nodo {
    struct Fichas ficha;
    struct Nodo* siguiente;
    struct Nodo* anterior;
};

struct Jugada {
    struct Nodo *cabeza;
    int tamanio;
};
//Nodo tablero
struct NodoTablero {
    struct NodoTablero *siguiente;
    struct NodoTablero *anterior;
    struct Jugada *lista;
};

struct Tablero {
    struct NodoTablero *cabeza;
    int tamanio;
};

struct Pila {
    int top;
    struct Fichas array[TAM_MAX];
};

struct Jugador {
    char nombre[50];
    bool esBot;
    struct Fichas mano[TAM_MAX];
    struct Jugador *siguiente;
    int numCartas;
    bool jugadorActivo;
};

struct ColaJugadores {
    struct Jugador *frente;
    struct Jugador *trasero;
};

#endif