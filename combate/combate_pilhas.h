#ifndef COMBATE_PILHAS_H
#define COMBATE_PILHAS_H
 
#include "glyph.h"
 
typedef struct NoMagia {
    Magia dado;
    struct NoMagia* prox;
} NoMagia;
 
void   magia_push(NoMagia** topo, Magia dado);
int    magia_pop(NoMagia** topo, Magia* saida);
void   magia_liberar(NoMagia* topo);

#define UNDO_MAX 10
 
typedef struct {
    char acoes[UNDO_MAX][100];
    int  topo;
} PilhaUndo;
 
void undo_iniciar(PilhaUndo* p);
int  undo_push(PilhaUndo* p, const char* acao);
int  undo_pop(PilhaUndo* p, char* saida);
 
#endif