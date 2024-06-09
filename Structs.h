#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <stddef.h>

#define TAM_MAX 106
#define TAM_PALO_MAX 26

struct Fichas {
    int numero;
    char *color;
    bool asignada;
};

struct colaCartas{
    int tamanio;
    struct Fichas *cabeza;
    struct Fichas *cola;
    struct Fichas arreglo[TAM_MAX];
    int indices[TAM_PALO_MAX];
};

struct Nodo {
    struct Fichas ficha;
    struct Nodo *siguiente;
    struct Nodo *anterior;
};

struct Jugada {
    struct Nodo *cabeza;
    int tamanio; 
    bool cerrada; //La jugada ya está completa
};

struct NodoTablero {
    struct NodoTablero *siguiente;
    struct NodoTablero *anterior;
    struct Jugada *jugada;
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
    bool jugadaRealizada;
};

struct ColaJugadores {
    struct Jugador *frente;
    struct Jugador *trasero;
};

#endif