#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "glyph.h"
#include "arvore.h"
#include "historico.h"

#define MAX_PERSONAGENS 6
#define TAM_NOME_LOCAL 50

typedef struct {
    Personagem lista[MAX_PERSONAGENS];
    int total;
} Grupo;

void grupo_iniciar(Grupo* g);

int grupo_adicionar(Grupo* g, Personagem p);

int persistencia_salvar_campanha(const char* caminho, Local* mundo, Grupo* grupo, Historico* hist);

int persistencia_carregar_campanha(const char* caminho, Local** mundo, Grupo* grupo, Historico* hist);

#endif