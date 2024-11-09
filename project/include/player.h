#ifndef __PLAYER_H
#define __PLAYER_H
#include "entity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void renderPlayerLife(Entity *player, SDL_Renderer *renderer, TTF_Font *font);

#endif // !__PLAYER_H