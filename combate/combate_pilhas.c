#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "combate_pilhas.h"


void magia_push(NoMagia** topo, Magia dado) {
    NoMagia* novo = malloc(sizeof(NoMagia));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente para empilhar magia.\n");
        return;
    }
    novo->dado = dado;
    novo->prox = *topo;
    *topo = novo;
}

int magia_pop(NoMagia** topo, Magia* saida) {
    if (*topo == NULL) {
        return 0;
    }
    NoMagia* antigo_topo = *topo;
    *saida = antigo_topo->dado;
    *topo = antigo_topo->prox;
    free(antigo_topo);
    return 1;
}

void magia_liberar(NoMagia* topo) {
    NoMagia* atual = topo;
    while (atual != NULL) {
        NoMagia* proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}


void undo_iniciar(PilhaUndo* p) {
    p->topo = -1;
}

int undo_push(PilhaUndo* p, const char* acao) {
    if (p->topo == UNDO_MAX - 1) {
        return 0;
    }
    p->topo++;
    strncpy(p->acoes[p->topo], acao, sizeof(p->acoes[p->topo]) - 1);
    p->acoes[p->topo][sizeof(p->acoes[p->topo]) - 1] = '\0';
    return 1;
}

int undo_pop(PilhaUndo* p, char* saida) {
    if (p->topo == -1) {
        return 0;
    }
    strcpy(saida, p->acoes[p->topo]);
    p->topo--;
    return 1;
}