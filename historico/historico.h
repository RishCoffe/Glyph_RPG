#ifndef HISTORICO_H
#define HISTORICO_H

#include "glyph.h"

typedef struct NoEvento {
    Evento dado;
    struct NoEvento* ant;
    struct NoEvento* prox;
} NoEvento;

typedef struct {
    NoEvento* inicio;
    NoEvento* fim;
    NoEvento* atual;
} Historico;

void historico_iniciar(Historico* h);
void historico_registrar(Historico* h, Evento dado);

Evento* historico_voltar(Historico* h);

Evento* historico_avancar(Historico* h);

void historico_liberar(Historico* h);

#endif