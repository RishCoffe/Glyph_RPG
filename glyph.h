#ifndef GLYPH_H
#define GLYPH_H

typedef struct {
    char nome[50];
    char descricao[100];
    int  valor;
    int  peso;
} Item;

typedef struct {
    char nome[50];
    int  hp;
    int  forca;
    int  nivel_desafio;
} Monstro;

typedef struct {
    char nome[50];
    char efeito[100];
    int  custo_mana;
} Magia;

typedef struct {
    char descricao[150];
    int  numero_turno;
} Evento;


typedef struct NoItem NoItem;
typedef struct Local Local;

typedef struct {
    char nome[50];
    int  hp, hp_max;
    int  forca, destreza, constituicao;
    int  inteligencia, sabedoria, carisma, iniciativa;
    NoItem* inventario;
    Local*  local_atual;
} Personagem;

#endif