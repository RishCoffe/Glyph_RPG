#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filas.h"

void fila_comandos_iniciar(FilaComandos* f) {
    f->inicio = 0;
    f->total = 0;
}

int fila_comandos_enqueue(FilaComandos* f, const char* cmd) {
    if (f->total == BUFFER_MAX) {
        return 0;
    }
    int posicao_fim = (f->inicio + f->total) % BUFFER_MAX;
    strncpy(f->comandos[posicao_fim], cmd, sizeof(f->comandos[posicao_fim]) - 1);
    f->comandos[posicao_fim][sizeof(f->comandos[posicao_fim]) - 1] = '\0';
    f->total++;
    return 1;
}

int fila_comandos_dequeue(FilaComandos* f, char* saida) {
    if (f->total == 0) {
        return 0;
    }
    strcpy(saida, f->comandos[f->inicio]);
    f->inicio = (f->inicio + 1) % BUFFER_MAX;
    f->total--;
    return 1;
}

void loot_iniciar(FilaLoot* f) {
    f->inicio = NULL;
    f->fim = NULL;
}

void loot_enqueue(FilaLoot* f, Item dado) {
    NoLoot* novo = malloc(sizeof(NoLoot));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente para gerar loot.\n");
        return;
    }
    novo->dado = dado;
    novo->prox = NULL;

    if (f->fim != NULL) {
        f->fim->prox = novo;
    } else {
        f->inicio = novo;
    }
    f->fim = novo;
}

int loot_dequeue(FilaLoot* f, Item* saida) {
    if (f->inicio == NULL) {
        return 0; 
    }
    NoLoot* removido = f->inicio;
    *saida = removido->dado;
    f->inicio = removido->prox;
    if (f->inicio == NULL) {
        f->fim = NULL;
    }
    free(removido);
    return 1;
}

void loot_liberar(FilaLoot* f) {
    NoLoot* atual = f->inicio;
    while (atual != NULL) {
        NoLoot* proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    f->inicio = f->fim = NULL;
}