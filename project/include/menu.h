#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>


typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Botao;


bool pontoDentroDoRetangulo(int x, int y, SDL_Rect *rect);
void renderizarBotao(SDL_Renderer *renderer, Botao *botao, TTF_Font *font, const char *text);
void renderizarTexto(SDL_Renderer *renderer, TTF_Font *font, const char *texto, SDL_Color cor, int x, int y);
void mostrarInstrucoes(SDL_Renderer *renderer, TTF_Font *font);
void showGameOverScreen(SDL_Renderer* renderer, TTF_Font* font);


#endif 
