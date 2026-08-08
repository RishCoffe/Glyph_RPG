#include <stdio.h>
#include <stdlib.h>
#include "historico.h"

void historico_iniciar(Historico* h) {
    h->inicio = NULL;
    h->fim = NULL;
    h->atual = NULL;
}

void historico_registrar(Historico* h, Evento dado) {
    NoEvento* novo = malloc(sizeof(NoEvento));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente para registrar evento.\n");
        return;
    }
    novo->dado = dado;
    novo->prox = NULL;
    novo->ant = h->fim;

    if (h->fim != NULL) {
        h->fim->prox = novo;
    } else {
        h->inicio = novo;
    }
    h->fim = novo;
    h->atual = novo; 
}

Evento* historico_voltar(Historico* h) {
    if (h->atual == NULL || h->atual->ant == NULL) {
        return NULL; 
    }
    h->atual = h->atual->ant;
    return &h->atual->dado;
}

Evento* historico_avancar(Historico* h) {
    if (h->atual == NULL || h->atual->prox == NULL) {
        return NULL;
    }
    h->atual = h->atual->prox;
    return &h->atual->dado;
}

void historico_liberar(Historico* h) {
    NoEvento* atual = h->inicio;
    while (atual != NULL) {
        NoEvento* proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    h->inicio = h->fim = h->atual = NULL;
}