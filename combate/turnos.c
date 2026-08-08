#include <stdio.h>
#include <stdlib.h>
#include "turnos.h"

void turnos_iniciar(ListaTurnos* l) {
    l->atual = NULL;
}

void turnos_inserir(ListaTurnos* l, Personagem* p) {
    NoTurno* novo = malloc(sizeof(NoTurno));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente para inserir combatente.\n");
        return;
    }
    novo->combatente = p;

    if (l->atual == NULL) {
        novo->prox = novo;
        l->atual = novo;
    } else {
        novo->prox = l->atual->prox;
        l->atual->prox = novo;
    }
}

Personagem* turnos_avancar(ListaTurnos* l) {
    if (l->atual == NULL) {
        return NULL;
    }
    l->atual = l->atual->prox;
    return l->atual->combatente;
}

int turnos_remover(ListaTurnos* l, Personagem* p) {
    if (l->atual == NULL) {
        return 0;
    }

    NoTurno* atual = l->atual;
    NoTurno* anterior = atual;

    do {
        if (atual->combatente == p) {
            if (atual->prox == atual) {
                l->atual = NULL;
            } else {
                anterior->prox = atual->prox;
                if (l->atual == atual) {
                    l->atual = atual->prox;
                }
            }
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    } while (atual != l->atual);

    return 0;
}

void turnos_liberar(ListaTurnos* l) {
    if (l->atual == NULL) {
        return;
    }
    NoTurno* atual = l->atual->prox;
    while (atual != l->atual) {
        NoTurno* proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    free(l->atual);
    l->atual = NULL;
}