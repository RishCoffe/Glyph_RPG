#ifndef FILAS_H
#define FILAS_H

#include "glyph.h"

#define BUFFER_MAX 20

typedef struct {
    char comandos[BUFFER_MAX][100];
    int  inicio;
    int  total;
} FilaComandos;

void fila_comandos_iniciar(FilaComandos* f);
int  fila_comandos_enqueue(FilaComandos* f, const char* cmd);
int  fila_comandos_dequeue(FilaComandos* f, char* saida);

typedef struct NoLoot {
    Item dado;
    struct NoLoot* prox;
} NoLoot;

typedef struct {
    NoLoot* inicio;
    NoLoot* fim;
} FilaLoot;

void loot_iniciar(FilaLoot* f);
void loot_enqueue(FilaLoot* f, Item dado);
int  loot_dequeue(FilaLoot* f, Item* saida);
void loot_liberar(FilaLoot* f);

#endif