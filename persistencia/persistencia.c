#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "inventario.h"

void grupo_iniciar(Grupo* g) {
    if (g != NULL) {
        g->total = 0;
    }
}

int grupo_adicionar(Grupo* g, Personagem p) {
    if (g == NULL || g->total >= MAX_PERSONAGENS) {
        return 0;
    }
    
    g->lista[g->total] = p;
    g->total++;
    
    return 1;
}

void local_salvar_no(FILE* f, Local* no) {
    if (f == NULL || no == NULL) {
        return;
    }

    fwrite(no->nome, sizeof(char), TAM_NOME_LOCAL, f);

    unsigned char tem_filho;
    if (no->primeiro_filho != NULL) {
        tem_filho = 1;
    } else {
        tem_filho = 0;
    }
    
    fwrite(&tem_filho, sizeof(unsigned char), 1, f);
    if (tem_filho) {
        local_salvar_no(f, no->primeiro_filho);
    }

    unsigned char tem_irmao;
    if (no->proximo_irmao != NULL) {
        tem_irmao = 1;
    } else {
        tem_irmao = 0;
    }

    fwrite(&tem_irmao, sizeof(unsigned char), 1, f);
    if (tem_irmao) {
        local_salvar_no(f, no->proximo_irmao);
    }
}


static Local* local_carregar_no(FILE* f) {
    char nome[TAM_NOME_LOCAL];
    fread(nome, sizeof(char), TAM_NOME_LOCAL, f);
    
    Local* no = arvore_criar_no(nome);

    unsigned char tem_filho;
    fread(&tem_filho, sizeof(unsigned char), 1, f);
    if (tem_filho) {
        no->primeiro_filho = local_carregar_no(f);
    }

    unsigned char tem_irmao;
    fread(&tem_irmao, sizeof(unsigned char), 1, f);
    if (tem_irmao) {
        no->proximo_irmao = local_carregar_no(f);
    }

    return no;
}

static void personagem_salvar(FILE* f, Personagem* p) {
    fwrite(p->nome, sizeof(char), TAM_NOME_LOCAL, f);
    fwrite(&p->hp,           sizeof(int), 1, f);
    fwrite(&p->hp_max,       sizeof(int), 1, f);
    fwrite(&p->forca,        sizeof(int), 1, f);
    fwrite(&p->destreza,     sizeof(int), 1, f);
    fwrite(&p->constituicao, sizeof(int), 1, f);
    fwrite(&p->inteligencia, sizeof(int), 1, f);
    fwrite(&p->sabedoria,    sizeof(int), 1, f);
    fwrite(&p->carisma,      sizeof(int), 1, f);
    fwrite(&p->iniciativa,   sizeof(int), 1, f);

    char nome_local[TAM_NOME_LOCAL] = "";
    if (p->local_atual != NULL) {
        strncpy(nome_local, p->local_atual->nome, TAM_NOME_LOCAL - 1);
        nome_local[TAM_NOME_LOCAL - 1] = '\0';
    }
    fwrite(nome_local, sizeof(char), TAM_NOME_LOCAL, f);

    int total_itens = 0;
    for (NoItem* atual = p->inventario; atual != NULL; atual = atual->prox) {
        total_itens++;
    }
    
    fwrite(&total_itens, sizeof(int), 1, f);
    for (NoItem* atual = p->inventario; atual != NULL; atual = atual->prox) {
        fwrite(&atual->dado, sizeof(Item), 1, f);
    }
}

static void personagem_carregar(FILE* f, Personagem* p, Local* mundo_raiz) {
    fread(p->nome, sizeof(char), TAM_NOME_LOCAL, f);
    fread(&p->hp,           sizeof(int), 1, f);
    fread(&p->hp_max,       sizeof(int), 1, f);
    fread(&p->forca,        sizeof(int), 1, f);
    fread(&p->destreza,     sizeof(int), 1, f);
    fread(&p->constituicao, sizeof(int), 1, f);
    fread(&p->inteligencia, sizeof(int), 1, f);
    fread(&p->sabedoria,    sizeof(int), 1, f);
    fread(&p->carisma,      sizeof(int), 1, f);
    fread(&p->iniciativa,   sizeof(int), 1, f);

    char nome_local[TAM_NOME_LOCAL];
    fread(nome_local, sizeof(char), TAM_NOME_LOCAL, f);

    if (nome_local[0] != '\0') {
        p->local_atual = arvore_buscar(mundo_raiz, nome_local);
    } else {
        p->local_atual = NULL;
    }

    p->inventario = NULL;
    int total_itens;
    fread(&total_itens, sizeof(int), 1, f);
    
    for (int i = 0; i < total_itens; i++) {
        Item item;
        fread(&item, sizeof(Item), 1, f);
        inventario_inserir(&p->inventario, item);
    }
}

static void historico_salvar(FILE* f, Historico* h) {
    int total = 0;
    for (NoEvento* atual = h->inicio; atual != NULL; atual = atual->prox) {
        total++;
    }
    
    fwrite(&total, sizeof(int), 1, f);
    for (NoEvento* atual = h->inicio; atual != NULL; atual = atual->prox) {
        fwrite(&atual->dado, sizeof(Evento), 1, f);
    }
}

static void historico_carregar(FILE* f, Historico* h) {
    historico_iniciar(h);
    int total;
    fread(&total, sizeof(int), 1, f);
    
    for (int i = 0; i < total; i++) {
        Evento e;
        fread(&e, sizeof(Evento), 1, f);
        historico_registrar(h, e);
    }
}

int persistencia_salvar_campanha(const char* caminho, Local* mundo, Grupo* grupo, Historico* hist) {
    FILE* f = fopen(caminho, "wb");
    if (f == NULL) {
        fprintf(stderr, "Erro de I/O: Nao foi possivel abrir '%s' para escrita.\n", caminho);
        return 0;
    }

    unsigned char tem_mundo;
    if (mundo != NULL) {
        tem_mundo = 1;
    } else {
        tem_mundo = 0;
    }

    fwrite(&tem_mundo, sizeof(unsigned char), 1, f);
    if (tem_mundo) {
        local_salvar_no(f, mundo);
    }

    fwrite(&grupo->total, sizeof(int), 1, f);
    for (int i = 0; i < grupo->total; i++) {
        personagem_salvar(f, &grupo->lista[i]);
    }

    historico_salvar(f, hist);

    fclose(f);
    return 1;
}

int persistencia_carregar_campanha(const char* caminho, Local** mundo, Grupo* grupo, Historico* hist) {
    FILE* f = fopen(caminho, "rb");
    if (f == NULL) {
        fprintf(stderr, "Erro de I/O: Nao foi possivel abrir '%s' para leitura.\n", caminho);
        return 0;
    }

    unsigned char tem_mundo;
    fread(&tem_mundo, sizeof(unsigned char), 1, f);

    if (tem_mundo) {
        *mundo = local_carregar_no(f);
    } else {
        *mundo = NULL;
    }

    grupo_iniciar(grupo);
    int total_personagens;
    fread(&total_personagens, sizeof(int), 1, f);
    
    for (int i = 0; i < total_personagens; i++) {
        Personagem p;
        personagem_carregar(f, &p, *mundo);
        grupo_adicionar(grupo, p);
    }

    historico_carregar(f, hist);

    fclose(f);
    return 1;
}