#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"
#include "map.h"
#include "enemy.h"
#include "menu.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erro ao inicializar SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Jogo com Menu Inicial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Criação dos botões do menu
    Botao botaoJogar = {{300, 200, 200, 50}, {0, 128, 255, 255}};  // Botão azul para "Jogar"
    Botao botaoSair = {{300, 300, 200, 50}, {255, 0, 0, 255}};     // Botão vermelho para "Sair"

    bool noMenu = true;
    bool running = true;

    // Loop do menu inicial
    while (noMenu) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                noMenu = false;
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (pontoDentroDoRetangulo(x, y, &botaoJogar.rect)) {
                    noMenu = false;  // Iniciar o jogo
                } else if (pontoDentroDoRetangulo(x, y, &botaoSair.rect)) {
                    noMenu = false;
                    running = false; // Sair do jogo
                }
            }
        }

        // Renderização do menu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fundo preto
        SDL_RenderClear(renderer);

        renderizarBotao(renderer, &botaoJogar);
        renderizarBotao(renderer, &botaoSair);
        SDL_RenderPresent(renderer);
    }

    if (running) {
        // Inicializa o jogador e entra no loop principal do jogo
        Player player;
        initPlayer(&player, renderer);

        double gravity = 500.0;
        double deltaTime = 0.09; // Aproximadamente 60 FPS
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
                PlayerAction action = handlePlayerInput(&event, &player);
                printf("Ação do jogador: %d\n", action);
            }

            updatePlayer(&player, gravity, deltaTime, renderer);

            // Implementação da lógica do chão e limites da tela
            if (player.position.y + player.height >= GROUND_LEVEL) {
                player.position.y = GROUND_LEVEL - player.height;
                player.position.velY = 0;
                player.position.onGround = true;
            } else {
                player.position.onGround = false;
            }

            if (player.position.x < 0) {
                player.position.x = 0;
            } else if (player.position.x + player.width > SCREEN_WIDTH) {
                player.position.x = SCREEN_WIDTH - player.width;
            }

            SDL_RenderClear(renderer);

            // Desenha o chão
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect groundRect = {0, GROUND_LEVEL, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_LEVEL};
            SDL_RenderFillRect(renderer, &groundRect);

            renderPlayer(&player, renderer);
            SDL_RenderPresent(renderer);

            SDL_Delay(16);
        }

        for (int i = 0; i < player.totalFrames; i++) {
            SDL_DestroyTexture(player.animationFrames[i]);
        }
        free(player.animationFrames);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
