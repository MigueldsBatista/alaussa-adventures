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

int main(int argc, char* argv[]) {
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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* bloco_texture = loadTexture("project/assets/blocks/block.png", renderer); // Carregar textura do bloco

    // Carregar a fonte
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

    // Botões
    Botao botaoJogar = {{230, 100, 200, 50}, {0, 255, 0, 255}};
    Botao botaoInstrucoes = {{230, 200, 200, 50}, {255, 255, 0, 255}};
    Botao botaoSair = {{230, 300, 200, 50}, {255, 0, 0, 255}};

    bool noMenu = true;
    bool running = true;

    // Música de fundo
    Mix_Music* bgMusic = loadMusic("./project/assets/musica/musicaAED.mp3");
    if (bgMusic) {
        playMusic(bgMusic);
    }

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
                } else if (pontoDentroDoRetangulo(x, y, &botaoInstrucoes.rect)) {
                    mostrarInstrucoes(renderer, font);
                } else if (pontoDentroDoRetangulo(x, y, &botaoSair.rect)) {
                    noMenu = false;
                    running = false; // Sair do jogo
                }
            }
        }

        // Renderização do menu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fundo preto
        SDL_RenderClear(renderer);

        loadMap("./project/assets/map/map.txt");
        renderizarBotao(renderer, &botaoJogar, font, "Jogar");
        renderizarBotao(renderer, &botaoInstrucoes, font, "Comandos");
        renderizarBotao(renderer, &botaoSair, font, "Sair");
        SDL_RenderPresent(renderer);
    }

    if (running) {
        // Inicializa o jogador
        Entity player;
        initEntity(&player, PLAYER, 50, 50, 3, renderer);



        while (running) {
            SDL_Event event;
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
                            }
                        }
                    }
                }

                Action action = handleEntityInput(&event, &player);
                printf("Ação do jogador: %d\n", action);
            }

            updateEntity(&player, renderer);
            updateEnemies(renderer);

            // Implementação da lógica do chão e limites da tela
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

            // Desenha o chão
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect groundRect = {0, GROUND_LEVEL, SCREEN_WIDTH, SCREEN_HEIGHT - GROUND_LEVEL};
            SDL_RenderFillRect(renderer, &groundRect);

            renderMap(renderer, bloco_texture);

            // Desenha o jogador
            renderEntity(&player, renderer);

            // Renderiza os inimigos
            renderEnemies(renderer);

            SDL_RenderPresent(renderer);

            SDL_Delay(16);
        }

        // Libera recursos do jogador
        for (int i = 0; i < player.totalFrames; i++) {
            SDL_DestroyTexture(player.animationFrames[i]);
        }
        free(player.animationFrames);

        // Libera recursos dos inimigos
        freeEnemyList();
    }

    // Limpeza final
    freeMusic(bgMusic);
    closeAudio();
    TTF_CloseFont(font); // Fechar a fonte
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); // Encerrar SDL_ttf
    IMG_Quit();
    SDL_Quit();

    return 0;
}
