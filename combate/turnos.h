#ifndef TURNOS_H
#define TURNOS_H

#include "glyph.h"

typedef struct NoTurno {
    Personagem* combatente;
    struct NoTurno* prox;
} NoTurno;

typedef struct {
    NoTurno* atual;
} ListaTurnos;

void turnos_iniciar(ListaTurnos* l);

void turnos_inserir(ListaTurnos* l, Personagem* p);

Personagem* turnos_avancar(ListaTurnos* l);

int turnos_remover(ListaTurnos* l, Personagem* p);

void turnos_liberar(ListaTurnos* l);

#endif