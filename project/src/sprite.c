#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "sprite.h"

void renderSprite(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest, SDL_Renderer* renderer){
//TODO: Implementar a renderização do sprite
return;

}

SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
        return NULL;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erro ao criar textura: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;
}

void updateSprite(Sprite* sprite){
//TODO: Implementar a atualização do sprite
return;
}

