CC = gcc
CFLAGS = -Wall -Iproject/include -g -O0
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Encontrar todos os arquivos .c no diretório 'project/src' e subdiretórios
SRCS = $(wildcard project/src/*.c)

# Gerar os arquivos .o a partir dos arquivos .c
OBJS = $(SRCS:.c=.o)

all: jogo

jogo: $(OBJS)
	$(CC) $(CFLAGS) -o jogo $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) jogo
