#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"

void inventario_inserir(NoItem** cabeca, Item dado) {
    NoItem* novo = malloc(sizeof(struct NoItem));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente para adicionar item.\n");
        return;
    }
    novo->dado = dado;
    novo->prox = *cabeca;
    *cabeca = novo;
}

int inventario_remover_por_nome(NoItem** cabeca, const char* nome) {
    NoItem* atual = *cabeca;
    NoItem* anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dado.nome, nome) == 0) {
            if (anterior == NULL) {
                *cabeca = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

void inventario_listar(NoItem* cabeca) {
    NoItem* atual = cabeca;
    if (atual == NULL) {
        printf("Inventario vazio.\n");
        return;
    }
    while (atual != NULL) {
        printf("- %s (peso %d, valor %d)\n",
               atual->dado.nome, atual->dado.peso, atual->dado.valor);
        atual = atual->prox;
    }
}

void inventario_liberar(NoItem* cabeca) {
    NoItem* atual = cabeca;
    while (atual != NULL) {
        NoItem* proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}