#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "sound.h"
#include "map.h"
#include "entity.h"
#include "menu.h"
#include "game.h"
#include "sprite.h"
#include "utils.h"
#include "enemy.h"
#include "player.h"

// Variáveis globais
bool noMenu = true;
bool running = true;
bool paused = false;
SDL_Renderer* renderer;
SDL_Event event;

int main(int argc, char* argv[]) {
    // Inicialização do SDL e módulos
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erro ao inicializar SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    if (!initAudio()) {
        return 1;
    }

    // Criação da janela e do renderizador
    SDL_Window* window = SDL_CreateWindow("Jogo com Menu Inicial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Carregamento da fonte
    TTF_Font* font = TTF_OpenFont("./project/assets/fontes/Open-Sans.ttf", 24);
    if (!font) {
        printf("Erro ao carregar fonte: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Carregamento e reprodução da música de fundo
    Mix_Music* bgMusic = loadMusic("./project/assets/musica/musicaAED.mp3");
    if (bgMusic) {
        playMusic(bgMusic);
    }

    // Loop do menu
    while (running) {
        noMenu = mostrarMenu(renderer, font);
        if (!running) break;
        // Chama o loop do jogo
        gameLoop(renderer, font);
    }


    // Libera os recursos do jogo
    freeMusic(bgMusic);
    closeAudio();
    TTF_CloseFont(font);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
