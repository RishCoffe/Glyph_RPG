# GLYPH — Sistema de Gerenciamento de RPG

Ferramenta de terminal para auxiliar o Mestre de Jogo na gestão de combates,
inventários, hierarquia de cenários e histórico de eventos em sessões de
RPG de mesa.

**Autor:** Tiago Arthur Vidoto Jorge — Matrícula 32511BSI015
**Disciplina:** Estrutura de Dados I

## Compilação

Requer `gcc` e `make`.

```bash
make
```

Isso gera o executável `glyph` na raiz do projeto.

Para compilar e já executar em seguida:

```bash
make run
```

Para limpar o executável gerado:

```bash
make clean
```

Se preferir compilar manualmente sem o Makefile:

```bash
gcc -Wall -Imundo -Ipersonagem -Icombate -Ihistorico -Ipersistencia -I. \
    -o glyph main.c mundo/arvore.c personagem/inventario.c \
    combate/combate.c combate/turnos.c combate/combate_pilhas.c \
    combate/filas.c historico/historico.c persistencia/persistencia.c
```

## Execução

```bash
./glyph
```

O programa abre um menu textual interativo. Navegue digitando o número da
opção desejada e pressionando Enter.

## Menu principal

1. **Gerenciamento do Mundo** — cria e navega a árvore de locais
   (Mundo/Reino/Cidade/Masmorra).
2. **Grupo de Personagens & Inventários** — cadastra personagens e gerencia
   o inventário de cada um.
3. **Sistema de Combate & Loot** — turnos, undo de ações, pilha de
   resolução de magias e distribuição de loot.
4. **Histórico de Eventos** — registra e navega (voltar/avançar) os eventos
   da campanha.
5. **Salvar/Carregar Campanha** — persiste o estado atual em arquivo
   binário e permite recarregar depois.
6. **Modo Debug (Fila de Comandos)** — enfileira e processa comandos de
   texto no buffer estático, simulando a fila de entrada do terminal.

## Estruturas de dados implementadas e onde encontrá-las

| Estrutura                  | Módulo                              | Funcionalidade                        |
|-----------------------------|--------------------------------------|----------------------------------------|
| Lista Simplesmente Encadeada| `personagem/inventario.c`            | Inventário de itens do personagem      |
| Lista Duplamente Encadeada  | `historico/historico.c`              | Histórico de eventos da sessão         |
| Lista Circular               | `combate/turnos.c`                   | Ciclo de turnos dos jogadores          |
| Pilha Dinâmica               | `combate/combate_pilhas.c`           | Pilha de resolução de magias           |
| Pilha Estática                | `combate/combate_pilhas.c`           | Undo de ações de combate               |
| Fila Estática                 | `combate/filas.c`                    | Buffer de comandos (Modo Debug)        |
| Fila Dinâmica                  | `combate/filas.c`                    | Fila de loot                           |
| Árvore Genérica                | `mundo/arvore.c`                     | Hierarquia de locais do mundo          |

## Persistência

O estado da campanha (mundo, grupo de personagens e histórico) pode ser
salvo em um arquivo binário através da opção 5 do menu principal, e
recarregado posteriormente na mesma ou em outra sessão.

## Roteiro de teste sugerido

1. Crie um local raiz e adicione sublocais (opção 1).
2. Cadastre 2-3 personagens e adicione itens ao inventário de cada um
   (opção 2).
3. Carregue os personagens nos turnos de combate e avance algumas vezes
   para ver o ciclo circular (opção 3 → 1 e 2).
4. Empilhe e resolva uma magia (opção 3 → 6 e 7).
5. Registre uma ação de undo (opção 3 → 3).
6. Enfileire e distribua um item de loot (opção 3 → 4 e 5).
7. Registre eventos e navegue entre eles (opção 4).
8. Salve a campanha, saia e recarregue para confirmar a persistência
   (opção 5).
9. Teste o buffer de comandos no Modo Debug (opção 6).
