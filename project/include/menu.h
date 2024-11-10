#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 50
extern char playerName[MAX_NAME_LENGTH];  // Variável para armazenar o nome do jogador

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char *texto;
} Botao;

typedef struct {
    SDL_Point posicao;
    SDL_Color cor;
    const char *texto;
} Texto;
void renderizarBotao(SDL_Renderer *renderer, Botao *botao, TTF_Font *font);
void renderizarTexto(SDL_Renderer *renderer, TTF_Font *font, Texto *texto);
void mostrarInstrucoes(SDL_Renderer *renderer, TTF_Font *font);
void showGameOverScreen(SDL_Renderer* renderer, TTF_Font* font);
bool mostrarMenu(SDL_Renderer* renderer, TTF_Font* font);

void mostrarMenuPausa(SDL_Renderer *renderer, TTF_Font *font);

void mostrarMenuFimDeJogo(SDL_Renderer *renderer, TTF_Font *font);
void capturarNomeJogador(SDL_Renderer *renderer, TTF_Font *font);


#endif
