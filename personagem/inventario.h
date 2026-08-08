#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "glyph.h"

struct NoItem {
    Item dado;
    struct NoItem* prox;
};

void inventario_inserir(NoItem** cabeca, Item dado);

int inventario_remover_por_nome(NoItem** cabeca, const char* nome);

void inventario_listar(NoItem* cabeca);

void inventario_liberar(NoItem* cabeca);

#endif