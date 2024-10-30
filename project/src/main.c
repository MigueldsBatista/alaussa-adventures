#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"
#include "map.h"

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

    SDL_Window* window = SDL_CreateWindow("Exemplo de Animação do Jogador", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player player;
    initPlayer(&player, renderer);

    int running = 1;
    double gravity = 500.0;
    double deltaTime = 0.09; // Aproximadamente 60 FPS
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            PlayerAction action = handlePlayerInput(&event, &player);
            printf("Ação do jogador: %d\n", action);
        }

        updatePlayer(&player, gravity, deltaTime);

        // Implementar lógica para o chão
        if (player.position.y + player.height >= GROUND_LEVEL) {
            player.position.y = GROUND_LEVEL - player.height; // Colocar o jogador na altura do chão
            player.position.velY = 0; // Resetar velocidade vertical
            player.position.onGround = true; // Indicar que o jogador está no chão
        } else {
            player.position.onGround = false; // O jogador não está no chão
        }

        // Lógica para limitar o movimento do jogador nas bordas da tela
        if (player.position.x < 0) {
            player.position.x = 0; // Impedir que o jogador saia pela esquerda
        } else if (player.position.x + player.width > SCREEN_WIDTH) {
            player.position.x = SCREEN_WIDTH - player.width; // Impedir que o jogador saia pela direita
        }

        SDL_RenderClear(renderer);
        
        // Desenhar o chão
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Cor verde para o chão
        SDL_Rect groundRect = {0, GROUND_LEVEL, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_LEVEL};
        SDL_RenderFillRect(renderer, &groundRect);

        renderPlayer(&player, renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    // Limpeza de recursos
    for (int i = 0; i < player.totalFrames; i++) {
        SDL_DestroyTexture(player.animationFrames[i]);
    }
    free(player.animationFrames);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
