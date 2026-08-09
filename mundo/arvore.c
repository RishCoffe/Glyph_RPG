#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

Local* arvore_criar_no(const char* nome) {
    Local* novo = malloc(sizeof(struct Local));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente para criar local.\n");
        return NULL;
    }
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    novo->primeiro_filho = NULL;
    novo->proximo_irmao = NULL;
    return novo;
}

void arvore_inserir_filho(Local* pai, Local* filho) {
    if (pai == NULL || filho == NULL) {
        return;
    }
    if (pai->primeiro_filho == NULL) {
        pai->primeiro_filho = filho;
        return;
    }
    Local* andar = pai->primeiro_filho;
    while (andar->proximo_irmao != NULL) {
        andar = andar->proximo_irmao;
    }
    andar->proximo_irmao = filho;
}

Local* arvore_buscar(Local* no, const char* nome) {
    if (no == NULL) {
        return NULL;
    }
    if (strcmp(no->nome, nome) == 0) {
        return no;
    }

    Local* resultado = arvore_buscar(no->primeiro_filho, nome);
    if (resultado != NULL) {
        return resultado;
    }

    return arvore_buscar(no->proximo_irmao, nome);
}

void arvore_imprimir(Local* no, int nivel) {
    if (no == NULL) {
        return;
    }

    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }
    printf("- %s\n", no->nome);

    arvore_imprimir(no->primeiro_filho, nivel + 1);
    arvore_imprimir(no->proximo_irmao, nivel);
}

void arvore_liberar(Local* no) {
    if (no == NULL) {
        return;
    }
    arvore_liberar(no->primeiro_filho);
    arvore_liberar(no->proximo_irmao);
    free(no);
}