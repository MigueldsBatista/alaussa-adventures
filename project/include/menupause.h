#ifndef MENUPAUSE_H
#define MENUPAUSE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "menu.h"
#include "menupause.h"





extern bool running;
extern bool paused;


void mostrarMenuPausa(SDL_Renderer *renderer, TTF_Font *font);


void verificarPausa(SDL_Renderer *renderer, TTF_Font *font);

#endif 
