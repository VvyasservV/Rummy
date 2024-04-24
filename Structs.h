#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdbool.h>
#define TAM_MAX 106

struct Fichas {
    int numero;
    char *color;
    bool asignada;
};

struct Nodo {
    struct Fichas ficha;
    struct Nodo* siguiente;
    struct Nodo* anterior;
};

struct ListaDoble {
    struct Nodo* cabeza;
    struct Nodo* cola;
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
};

struct ColaJugadores {
    struct Jugador *frente;
    struct Jugador *trasero;
};

#endif