#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glyph.h"
#include "arvore.h"
#include "historico.h"
#include "inventario.h"
#include "turnos.h"
#include "combate_pilhas.h"
#include "filas.h"
#include "combate.h"
#include "persistencia.h"

void limpar_tela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar(void) {
    printf("\nPressione Enter para continuar...");
    getchar();
}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
    Local* mundo = NULL;
    Grupo grupo;
    Historico historico;
    EstadoCombate combate;

    grupo_iniciar(&grupo);
    historico_iniciar(&historico);
    combate_iniciar(&combate);

    int opcao_principal = -1;

    while (opcao_principal != 0) {
        limpar_tela();
        printf("===========================================\n");
        printf("                GLYPH                      \n");
        printf("===========================================\n");
        printf("1. Gerenciamento do Mundo (Arvore de Locais)\n");
        printf("2. Grupo de Personagens & Inventarios\n");
        printf("3. Sistema de Combate & Loot\n");
        printf("4. Historico de Eventos (Navegacao)\n");
        printf("5. Salvar / Carregar Campanha\n");
        printf("0. Sair do Programa\n");
        printf("===========================================\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao_principal) != 1) {
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao_principal) {
            case 1: {  
                int sub_op = -1;
                while (sub_op != 0) {
                    limpar_tela();
                    printf("--- GERENCIAMENTO DO MUNDO (ARVORE) ---\n");
                    printf("1. Imprimir Mapa do Mundo\n");
                    printf("2. Criar Local Raiz\n");
                    printf("3. Adicionar Sublocal (Filho)\n");
                    printf("0. Voltar ao Menu Principal\n");
                    printf("---------------------------------------\n");
                    printf("Escolha uma opcao: ");

                    if (scanf("%d", &sub_op) != 1) {
                        limpar_buffer();
                        continue;
                    }
                    limpar_buffer();

                    limpar_tela();
                    if (sub_op == 1) {
                        printf("=== MAPA DO MUNDO ===\n");
                        if (mundo == NULL) {
                            printf("Nenhum mundo criado ainda.\n");
                        } else {
                            arvore_imprimir(mundo, 0);
                        }
                        pausar();
                    } else if (sub_op == 2) {
                        if (mundo != NULL) {
                            printf("Ja existe um mundo criado! (%s)\n", mundo->nome);
                        } else {
                            char nome[50];
                            printf("Nome da regiao principal/raiz: ");
                            fgets(nome, sizeof(nome), stdin);
                            nome[strcspn(nome, "\n")] = '\0';

                            mundo = arvore_criar_no(nome);
                            printf("Regiao '%s' criada como raiz do mundo.\n", nome);
                        }
                        pausar();
                    } else if (sub_op == 3) {
                        if (mundo == NULL) {
                            printf("Crie a raiz do mundo primeiro.\n");
                        } else {
                            char pai_nome[50], filho_nome[50];
                            printf("Nome da regiao pai: ");
                            fgets(pai_nome, sizeof(pai_nome), stdin);
                            pai_nome[strcspn(pai_nome, "\n")] = '\0';

                            Local* pai = arvore_buscar(mundo, pai_nome);
                            if (pai == NULL) {
                                printf("Local pai '%s' nao foi encontrado.\n", pai_nome);
                            } else {
                                printf("Nome da nova sub-regiao: ");
                                fgets(filho_nome, sizeof(filho_nome), stdin);
                                filho_nome[strcspn(filho_nome, "\n")] = '\0';

                                Local* filho = arvore_criar_no(filho_nome);
                                arvore_inserir_filho(pai, filho);
                                printf("Sublocal '%s' adicionado em '%s'.\n", filho_nome, pai_nome);
                            }
                        }
                        pausar();
                    }
                }
                break;
            }

            case 2: {
                int sub_op = -1;
                while (sub_op != 0) {
                    limpar_tela();
                    printf("--- GRUPO & INVENTARIOS ---\n");
                    printf("1. Listar Integrantes do Grupo\n");
                    printf("2. Adicionar Personagem ao Grupo\n");
                    printf("3. Gerenciar Inventario de um Personagem\n");
                    printf("0. Voltar ao Menu Principal\n");
                    printf("---------------------------\n");
                    printf("Escolha uma opcao: ");

                    if (scanf("%d", &sub_op) != 1) {
                        limpar_buffer();
                        continue;
                    }
                    limpar_buffer();

                    limpar_tela();
                    if (sub_op == 1) {
                        printf("=== INTEGRANTES DO GRUPO (%d/%d) ===\n", grupo.total, MAX_PERSONAGENS);
                        for (int i = 0; i < grupo.total; i++) {
                            printf("[%d] %s - HP: %d/%d | Local: %s\n", 
                                   i + 1, 
                                   grupo.lista[i].nome, 
                                   grupo.lista[i].hp, 
                                   grupo.lista[i].hp_max,
                                   grupo.lista[i].local_atual ? grupo.lista[i].local_atual->nome : "Desconhecido");
                        }
                        pausar();
                    } else if (sub_op == 2) {
                        if (grupo.total >= MAX_PERSONAGENS) {
                            printf("Grupo cheio (maximo %d personagens).\n", MAX_PERSONAGENS);
                        } else {
                            Personagem p;
                            memset(&p, 0, sizeof(Personagem));
                            printf("Nome do Personagem: ");
                            fgets(p.nome, sizeof(p.nome), stdin);
                            p.nome[strcspn(p.nome, "\n")] = '\0';

                            printf("HP Maximo: ");
                            scanf("%d", &p.hp_max);
                            p.hp = p.hp_max;

                            printf("Iniciativa: ");
                            scanf("%d", &p.iniciativa);
                            limpar_buffer();

                            p.local_atual = mundo;
                            p.inventario = NULL;

                            grupo_adicionar(&grupo, p);
                            printf("Personagem '%s' adicionado com sucesso!\n", p.nome);
                        }
                        pausar();
                    } else if (sub_op == 3) {
                        if (grupo.total == 0) {
                            printf("Nenhum personagem cadastrado.\n");
                            pausar();
                        } else {
                            printf("Selecione o personagem pelo numero:\n");
                            for (int i = 0; i < grupo.total; i++) {
                                printf("%d. %s\n", i + 1, grupo.lista[i].nome);
                            }
                            int idx;
                            scanf("%d", &idx);
                            limpar_buffer();

                            if (idx < 1 || idx > grupo.total) {
                                printf("Indice invalido.\n");
                                pausar();
                            } else {
                                Personagem* p_sel = &grupo.lista[idx - 1];
                                int inv_op = -1;
                                while (inv_op != 0) {
                                    limpar_tela();
                                    printf("--- INVENTARIO DE %s ---\n", p_sel->nome);
                                    printf("1. Listar Itens\n");
                                    printf("2. Adicionar Item\n");
                                    printf("3. Remover Item por Nome\n");
                                    printf("0. Voltar\n");
                                    printf("-------------------------\n");
                                    printf("Opcao: ");
                                    scanf("%d", &inv_op);
                                    limpar_buffer();

                                    limpar_tela();
                                    if (inv_op == 1) {
                                        printf("=== ITENS DE %s ===\n", p_sel->nome);
                                        inventario_listar(p_sel->inventario);
                                        pausar();
                                    } else if (inv_op == 2) {
                                        Item item;
                                        printf("Nome do Item: ");
                                        fgets(item.nome, sizeof(item.nome), stdin);
                                        item.nome[strcspn(item.nome, "\n")] = '\0';

                                        printf("Peso: ");
                                        scanf("%d", &item.peso);
                                        printf("Valor: ");
                                        scanf("%d", &item.valor);
                                        limpar_buffer();

                                        inventario_inserir(&p_sel->inventario, item);
                                        printf("Item '%s' adicionado ao inventario de %s.\n", item.nome, p_sel->nome);
                                        pausar();
                                    } else if (inv_op == 3) {
                                        char nome_item[50];
                                        printf("Nome do Item a remover: ");
                                        fgets(nome_item, sizeof(nome_item), stdin);
                                        nome_item[strcspn(nome_item, "\n")] = '\0';

                                        if (inventario_remover_por_nome(&p_sel->inventario, nome_item)) {
                                            printf("Item '%s' removido.\n", nome_item);
                                        } else {
                                            printf("Item nao encontrado.\n");
                                        }
                                        pausar();
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }

            case 3: {
                int sub_op = -1;
                while (sub_op != 0) {
                    limpar_tela();
                    printf("--- GERENCIAMENTO DE COMBATE ---\n");
                    printf("1. Carregar Personagens do Grupo no Combate (Turnos)\n");
                    printf("2. Avançar Turno do Combate\n");
                    printf("3. Empilhar Magia / Acao de Undo\n");
                    printf("4. Adicionar Item ao Fila de Loot\n");
                    printf("5. Distribuir Loot para Personagem\n");
                    printf("0. Voltar ao Menu Principal\n");
                    printf("--------------------------------\n");
                    printf("Escolha uma opcao: ");

                    if (scanf("%d", &sub_op) != 1) {
                        limpar_buffer();
                        continue;
                    }
                    limpar_buffer();

                    limpar_tela();
                    if (sub_op == 1) {
                        turnos_liberar(&combate.turnos);
                        turnos_iniciar(&combate.turnos);
                        for (int i = 0; i < grupo.total; i++) {
                            turnos_inserir(&combate.turnos, &grupo.lista[i]);
                        }
                        printf("Foram adicionados %d combatentes a roda de turnos.\n", grupo.total);
                        pausar();
                    } else if (sub_op == 2) {
                        Personagem* atual = turnos_avancar(&combate.turnos);
                        if (atual == NULL) {
                            printf("Nenhum combatente presente na roda de turnos.\n");
                        } else {
                            printf(">>> TURNO ATUAL: %s (HP: %d/%d) <<<\n", atual->nome, atual->hp, atual->hp_max);
                        }
                        pausar();
                    } else if (sub_op == 3) {
                        char acao[100];
                        printf("Acao de combate a registrar no Undo: ");
                        fgets(acao, sizeof(acao), stdin);
                        acao[strcspn(acao, "\n")] = '\0';

                        if (undo_push(&combate.undo, acao)) {
                            printf("Acao '%s' registrada na pilha de Undo.\n", acao);
                        } else {
                            printf("Pilha de Undo cheia.\n");
                        }
                        pausar();
                    } else if (sub_op == 4) {
                        Item item;
                        printf("Nome do Loot: ");
                        fgets(item.nome, sizeof(item.nome), stdin);
                        item.nome[strcspn(item.nome, "\n")] = '\0';

                        printf("Peso: ");
                        scanf("%d", &item.peso);
                        printf("Valor: ");
                        scanf("%d", &item.valor);
                        limpar_buffer();

                        loot_enqueue(&combate.loot, item);
                        printf("Loot '%s' enfileirado com sucesso.\n", item.nome);
                        pausar();
                    } else if (sub_op == 5) {
                        if (grupo.total == 0) {
                            printf("Nenhum personagem cadastrado para receber o loot.\n");
                        } else {
                            printf("Escolha o personagem para receber o Loot acumulado:\n");
                            for (int i = 0; i < grupo.total; i++) {
                                printf("%d. %s\n", i + 1, grupo.lista[i].nome);
                            }
                            int p_idx;
                            scanf("%d", &p_idx);
                            limpar_buffer();

                            if (p_idx >= 1 && p_idx <= grupo.total) {
                                combate_distribuir_loot(&combate, &grupo.lista[p_idx - 1]);
                            } else {
                                printf("Personagem invalido.\n");
                            }
                        }
                        pausar();
                    }
                }
                break;
            }

            case 4: {
                int sub_op = -1;
                while (sub_op != 0) {
                    limpar_tela();
                    printf("--- HISTORICO DE EVENTOS DA CAMPANHA ---\n");
                    printf("1. Registrar Novo Evento\n");
                    printf("2. Voltar Evento (Anterior)\n");
                    printf("3. Avancar Evento (Proximo)\n");
                    printf("0. Voltar ao Menu Principal\n");
                    printf("----------------------------------------\n");
                    printf("Escolha uma opcao: ");

                    if (scanf("%d", &sub_op) != 1) {
                        limpar_buffer();
                        continue;
                    }
                    limpar_buffer();

                    limpar_tela();
                    if (sub_op == 1) {
                        Evento e;
                        printf("Descricao do Evento: ");
                        fgets(e.descricao, sizeof(e.descricao), stdin);
                        e.descricao[strcspn(e.descricao, "\n")] = '\0';

                        historico_registrar(&historico, e);
                        printf("Evento registrado!\n");
                        pausar();
                    } else if (sub_op == 2) {
                        Evento* e = historico_voltar(&historico);
                        if (e != NULL) {
                            printf("=== EVENTO ANTERIOR ===\nDescricao: %s\n", e->descricao);
                        } else {
                            printf("Inicio do historico atingido ou nenhum evento gravado.\n");
                        }
                        pausar();
                    } else if (sub_op == 3) {
                        Evento* e = historico_avancar(&historico);
                        if (e != NULL) {
                            printf("=== PROXIMO EVENTO ===\nDescricao: %s\n", e->descricao);
                        } else {
                            printf("Fim do historico atingido.\n");
                        }
                        pausar();
                    }
                }
                break;
            }

            case 5: {
                int sub_op = -1;
                while (sub_op != 0) {
                    limpar_tela();
                    printf("--- SALVAR / CARREGAR CAMPANHA ---\n");
                    printf("1. Salvar Campanha em Arquivo\n");
                    printf("2. Carregar Campanha de Arquivo\n");
                    printf("0. Voltar ao Menu Principal\n");
                    printf("----------------------------------\n");
                    printf("Escolha uma opcao: ");

                    if (scanf("%d", &sub_op) != 1) {
                        limpar_buffer();
                        continue;
                    }
                    limpar_buffer();

                    limpar_tela();
                    if (sub_op == 1) {
                        char caminho[100];
                        printf("Nome do arquivo de destino (ex: save.bin): ");
                        fgets(caminho, sizeof(caminho), stdin);
                        caminho[strcspn(caminho, "\n")] = '\0';

                        if (persistencia_salvar_campanha(caminho, mundo, &grupo, &historico)) {
                            printf("Campanha salva com sucesso em '%s'!\n", caminho);
                        } else {
                            printf("Falha ao salvar campanha.\n");
                        }
                        pausar();
                    } else if (sub_op == 2) {
                        char caminho[100];
                        printf("Nome do arquivo para carregar (ex: save.bin): ");
                        fgets(caminho, sizeof(caminho), stdin);
                        caminho[strcspn(caminho, "\n")] = '\0';

                        if (mundo != NULL) {
                            arvore_liberar(mundo);
                            mundo = NULL;
                        }

                        if (persistencia_carregar_campanha(caminho, &mundo, &grupo, &historico)) {
                            printf("Campanha '%s' carregada com sucesso!\n", caminho);
                        } else {
                            printf("Falha ao carregar campanha.\n");
                        }
                        pausar();
                    }
                }
                break;
            }

            case 0:
                limpar_tela();
                printf("Encerrando o programa Glyph...\n");
                break;

            default:
                break;
        }
    }

    combate_finalizar(&combate);
    historico_liberar(&historico);
    if (mundo != NULL) {
        arvore_liberar(mundo);
    }

    for (int i = 0; i < grupo.total; i++) {
        inventario_liberar(grupo.lista[i].inventario);
    }

    return 0;
}