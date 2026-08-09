#include <stdio.h>
#include "combate.h"
#include "inventario.h"

void combate_iniciar(EstadoCombate* c) {
    turnos_iniciar(&c->turnos);
    c->pilha_magias = NULL;
    undo_iniciar(&c->undo);
    loot_iniciar(&c->loot);
    fila_comandos_iniciar(&c->buffer_comandos);
}

void combate_finalizar(EstadoCombate* c) {
    turnos_liberar(&c->turnos);
    magia_liberar(c->pilha_magias);
    c->pilha_magias = NULL;
    loot_liberar(&c->loot);
}

void combate_distribuir_loot(EstadoCombate* c, Personagem* p) {
    Item item;
    while (loot_dequeue(&c->loot, &item)) {
        inventario_inserir(&p->inventario, item);
        printf("%s recebeu: %s\n", p->nome, item.nome);
    }
}