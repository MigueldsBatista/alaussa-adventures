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

void initGame(SDL_Renderer* renderer) {
    //TODO: Implementar a inicialização do jogo
    //initEnemy(); // Inicializa inimigos
    loadMap("project/assets/images/parallax.png"); // Carrega o mapa
}


void gameLoop(SDL_Renderer* renderer, TTF_Font* font) {
    Uint32 lastTime = SDL_GetTicks();
    Entity player;
    extern bool running;
    extern bool paused;
    extern SDL_Event event;
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
                // Pausa o jogo
                paused = true;
                while (paused) {
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                            paused = !paused;
                        }
                        if (paused) {
                            mostrarMenuPausa(renderer, font);
                        }
                    }
                }
            }
            handleEntityInput(&event, &player);
        }

        // Atualiza o jogador e inimigos
        updateEntity(&player, renderer);
        updateEnemies(renderer);
        atualizarImortalidade(&player, deltaTime);

        // Verificação de colisões
        checkCoinCollected(&player, renderer);
        checkPlayerEnemyCollision(&player, &enemyList);

        // Restringe o jogador ao limite da tela e ao solo
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

        if (checkPlayerInFinishPosition(&player, renderer, font)) {
            mostrarMenuFimDeJogo(renderer, font);
            writePlayerInfo(&player, playerName);
            return;  // Retorna para o loop principal do jogo
        }

        // Renderiza a tela
        SDL_RenderClear(renderer);

        // Desenha o chão
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect groundRect = {0, GROUND_LEVEL, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_LEVEL};
        SDL_RenderFillRect(renderer, &groundRect);

        // Renderiza o mapa, o jogador e os inimigos
        renderMap(renderer);
        renderEntity(&player, renderer);
        renderEnemies(renderer);

        // Verifica se o jogador morreu
        if (!player.isAlive) {
            showGameOverScreen(renderer, font);
            writePlayerInfo(&player, playerName);
            SDL_Delay(50);
            return;  // Retorna ao menu
        }

        // Renderiza informações de vida e moedas
        renderPlayerLife(&player, renderer, font);
        renderPlayerCoins(&player, renderer, font);

        // Verifica transição de mapa
        checkMapTransition(&player, renderer);

        // Exibe a renderização
        SDL_RenderPresent(renderer);

        // Controla a taxa de quadros
        SDL_Delay(16);
    }

    // Libera os recursos do jogador
    for (int i = 0; i < player.totalFrames; i++) {
        SDL_DestroyTexture(player.animationFrames[i]);
    }
    free(player.animationFrames);
    freeEnemyList();
}

void shutdownGame(){
//TODO: Implementar a finalização do jogo    
return;
}
