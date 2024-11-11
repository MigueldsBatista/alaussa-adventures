#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "sprite.h"
#include "map.h"
#include "enemy.h"
#include "menu.h"
#include "player.h"
#include <SDL2/SDL_ttf.h>


Entity player;
SDL_Texture *background;


void gameLoop(SDL_Renderer* renderer, TTF_Font* font) {
    Uint32 lastTime = SDL_GetTicks();
    extern bool running;
    extern bool paused;
    extern SDL_Event event;

    background = loadTexture("project/assets/map/background_game.png", renderer, "background");
    loadMap("project/assets/map/map_0.txt");

    // Inicializa o jogador
    initPlayer(&player, PLAYER, 50, 50, 3, renderer);

    // Loop principal do jogo
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Tratamento de eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                paused = true;
                while (paused) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                            paused = false;
                        }
                        if (paused) {
                            mostrarMenuPausa(renderer, font);
                        }
                    }
                }
            }
            handleEntityInput(&event, &player);
        }

        // Atualiza o jogador, inimigos e a lógica do jogo
        updateEntity(&player, renderer);
        updateEnemies(renderer);
        atualizarImortalidade(&player, deltaTime);

        // Verificação de colisões
        checkCoinCollected(&player, renderer);
        checkPlayerEnemyCollision(&player, &enemyList);
        checkMapTransition(&player, renderer);

        // Limita o jogador ao chão e às bordas da tela
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

        // Checa se o jogador chegou ao final do mapa
        if (checkPlayerInFinishPosition(&player, renderer, font)) {
            mostrarMenuFimDeJogo(renderer, font);
            writePlayerInfo(&player, playerName);
            return;
        }

        // Verifica se o jogador morreu
        if (!player.isAlive) {
            showGameOverScreen(renderer, font);
            writePlayerInfo(&player, playerName);
            SDL_Delay(50);
            return;
        }

        // Renderização
        SDL_RenderClear(renderer);

        // Renderiza o fundo
        renderBackground(renderer, background, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Renderiza o mapa
        renderMap(renderer);

        // Desenha o chão
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect groundRect = {0, GROUND_LEVEL, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_LEVEL};
        SDL_RenderFillRect(renderer, &groundRect);

        // Renderiza o jogador e os inimigos
        renderEntity(&player, renderer);
        renderEnemies(renderer);

        // Renderiza informações de vida e moedas
        renderPlayerLife(&player, renderer, font);
        renderPlayerCoins(&player, renderer, font);

        // Atualiza a tela
        SDL_RenderPresent(renderer);

        // Simula o movimento da câmera

        // Controla a taxa de quadros (60 FPS)
        SDL_Delay(16);
    }
   

}

void shutdownGame() {
    // Libera os quadros de animação do jogador
    for (int i = 0; i < player.totalFrames; i++) {
        if (player.animationFrames[i] != NULL) {
            SDL_DestroyTexture(player.animationFrames[i]);
            player.animationFrames[i] = NULL;
        }
    }

    // Libera a memória da animação do jogador
    if (player.animationFrames != NULL) {
        free(player.animationFrames);
        player.animationFrames = NULL;
    }
    
    freeEnemyList();
 
    // Libera a textura de fundo
    if (background != NULL) {
        SDL_DestroyTexture(background);
        background = NULL;
    }
}