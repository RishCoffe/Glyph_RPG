#ifndef ARVORE_H
#define ARVORE_H

#include "glyph.h"


struct Local {
    char nome[50];
    struct Local* primeiro_filho;
    struct Local* proximo_irmao;
};

Local* arvore_criar_no(const char* nome);

void arvore_inserir_filho(Local* pai, Local* filho);

Local* arvore_buscar(Local* no, const char* nome);

void arvore_imprimir(Local* no, int nivel);

void arvore_liberar(Local* no);

#endif