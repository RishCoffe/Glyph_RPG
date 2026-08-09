#ifndef COMBATE_H
#define COMBATE_H

#include "glyph.h"
#include "turnos.h"
#include "combate_pilhas.h"
#include "filas.h"

typedef struct {
    ListaTurnos turnos;
    NoMagia*    pilha_magias;
    PilhaUndo   undo;
    FilaLoot    loot;
    FilaComandos buffer_comandos;
} EstadoCombate;

void combate_iniciar(EstadoCombate* c);
void combate_finalizar(EstadoCombate* c);

void combate_distribuir_loot(EstadoCombate* c, Personagem* p);

#endif