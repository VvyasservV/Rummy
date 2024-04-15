#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdbool.h>
#define TAM_MAX 106

struct Fichas {
    int numero;
    char *color;
    bool asignada;
};

struct Pila {
    int top;
    struct Fichas array[TAM_MAX];
};

struct Jugador {
    char nombre[50];
    bool esBot;
    struct Fichas mano[13];
    struct Jugador *siguiente;
};

struct ColaJugadores {
    struct Jugador *frente;
    struct Jugador *trasero;
};

#endif