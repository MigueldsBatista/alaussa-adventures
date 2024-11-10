#ifndef MENU_H
#define MENU_H


#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Botao;

void renderizarBotao(SDL_Renderer *renderer, Botao *botao, TTF_Font *font, const char *text);

void mostrarInstrucoes(SDL_Renderer *renderer, TTF_Font *font);

void showGameOverScreen(SDL_Renderer *renderer, TTF_Font *font);

bool mostrarMenu(SDL_Renderer* renderer, TTF_Font* font);

#endif