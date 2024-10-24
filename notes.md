Vou usar esse arquivo pra comunicar algumas coisas pra vcs lerem

pra compilar o jogo, eu criei um Makefile

COMANDOS IMPORTANTES !

make //vai fazer o build do nosso jogo compilando todos os arquivos .h com .c no main

make clen //deleta os arquivos .o que foram criados


Variáveis do Makefile:

CC = gcc: Define o compilador que será usado (no caso, o gcc).
CFLAGS = -Wall -Iproject/include:
-Wall ativa avisos de possíveis problemas no código.
-Iproject/include adiciona o diretório include no caminho de inclusão, onde estão os arquivos .h.
LIBS = -lSDL2 -lSDL2_image: Especifica as bibliotecas que o projeto usa. Aqui, são as bibliotecas SDL2 e SDL2_image.
Fontes e Objetos:

SRCS: Lista os arquivos fonte .c localizados em project/src/.
OBJS: Transforma os arquivos .c listados em SRCS para os arquivos .o correspondentes. A expressão $(SRCS:.c=.o) substitui a extensão .c por .o.
Regras:

all: jogo: Essa é a regra padrão que será executada ao rodar make. Ela indica que o objetivo principal é criar o executável jogo.

jogo: $(OBJS): Define que o executável jogo depende dos arquivos objeto listados em $(OBJS). Se os objetos mudarem ou não existirem, ele recompilará os arquivos fontes e criará o executável.

O comando seguinte: $(CC) $(CFLAGS) -o jogo $(OBJS) $(LIBS) compila os objetos e gera o executável jogo, linkando as bibliotecas especificadas em LIBS.

Limpeza:

clean:: Remove os arquivos objetos .o e o executável jogo. O comando rm -f $(OBJS) jogo faz isso de forma silenciosa (sem gerar erros se os arquivos não existirem).