#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "sprite.h"

void renderSprite(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest, SDL_Renderer* renderer){
//TODO: Implementar a renderização do sprite
return;

}

SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer, const char* name){
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        printf("Erro ao carregar a imagem: %s\n", name);
        return NULL;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erro ao criar textura: %s\n", name);
    }

    SDL_FreeSurface(surface);
    return texture;
}


