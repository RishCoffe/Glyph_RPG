CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Imundo -Ipersonagem -Icombate -Ihistorico -Ipersistencia -I.
SRC = main.c mundo/arvore.c personagem/inventario.c combate/combate.c \
      combate/turnos.c combate/combate_pilhas.c combate/filas.c \
      historico/historico.c persistencia/persistencia.c
TARGET = glyph

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
