#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
void delay(unsigned int frame_limit){
//TODO: Implementar a função delay
    SDL_Delay(frame_limit);

}

SDL_Texture* loadImage(const char* filename, SDL_Renderer* renderer){
//TODO: Implementar a função loadImage
    // Carrega uma imagem e retorna a textura
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filename);
    if (!texture) {
        printf("Erro ao carregar imagem: %s\n", IMG_GetError());
    }
    return texture;
}
