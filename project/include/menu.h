#ifndef MENU_H
#define MENU_H


#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Botao;

bool pontoDentroDoRetangulo(int x, int y, SDL_Rect *rect);

void renderizarBotao(SDL_Renderer *renderer, Botao *botao);

#endif