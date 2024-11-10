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
#include "enemy.h"  // Inclusão do arquivo de inimigos
#include "player.h" // Inclusão do arquivo de jogador
#include "menupause.h"

bool noMenu = true;
bool running = true;
SDL_Renderer* renderer;

// Declaração da função de menu
bool mostrarMenu(SDL_Renderer* renderer, TTF_Font* font);

int main(int argc, char* argv[]) {

    Uint32 lastTime = SDL_GetTicks();
    SDL_Event event;

    // Inicialização do SDL
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

    SDL_Window* window = SDL_CreateWindow("Jogo com Menu Inicial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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

    Mix_Music* bgMusic = loadMusic("./project/assets/musica/musicaAED.mp3");
    if (bgMusic) {
        playMusic(bgMusic);
    }

    // Loop principal
    while (running) {
        noMenu = mostrarMenu(renderer, font);

        if (!running) break;

        Entity player;
        initPlayer(&player, PLAYER, 50, 50, 3, renderer);

        while (running) {
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                    printf("Jogo pausado. Pressione P para despausar.\n");
                    bool paused = true;
                    while (paused) {
                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                                paused = false;
                                if(paused){
                                    verificarPausa(renderer, font);
                                }
                            }
                        }
                    }
                }
                handleEntityInput(&event, &player);
            }

            updateEntity(&player, renderer);
            updateEnemies(renderer);
            atualizarImortalidade(&player, deltaTime);
            checkCoinCollected(&player, renderer);
            checkPlayerEnemyCollision(&player, &enemyList);

            if (player.position.y + player.height >= GROUND_LEVEL) {
                player.position.y = GROUND_LEVEL - player.height;
                player.position.velY = 0;
                player.position.onGround = true;
            }

            if (player.position.x < 0) {
                player.position.x = 0;
            } else if (player.position.x + player.width > SCREEN_WIDTH) {
                player.position.x = SCREEN_WIDTH - player.width;
            }

            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect groundRect = {0, GROUND_LEVEL, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_LEVEL};
            SDL_RenderFillRect(renderer, &groundRect);

            renderMap(renderer);
            renderEntity(&player, renderer);
            renderEnemies(renderer);

            if (!player.isAlive) {
                showGameOverScreen(renderer, font);
                SDL_Delay(50);
                break; // Sai do loop de jogo e volta ao menu
            }

            renderPlayerLife(&player, renderer, font);
            renderPlayerCoins(&player, renderer, font);
            checkMapTransition(&player, renderer);
            SDL_RenderPresent(renderer);

            SDL_Delay(7);
        }

        for (int i = 0; i < player.totalFrames; i++) {
            SDL_DestroyTexture(player.animationFrames[i]);
        }
        free(player.animationFrames);
        freeEnemyList();
    }

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
