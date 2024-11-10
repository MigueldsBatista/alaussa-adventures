#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "menupause.h"

// Variável para controlar o estado de pausa
bool paused = false;


// Função para mostrar o menu de pausa
void mostrarMenuPausa(SDL_Renderer *renderer, TTF_Font *font) {
    Botao botaoContinuar = {{230, 300, 200, 50}, {0, 255, 0, 255}}; // Botão para continuar
    bool menuPausaAtivo = true;

    while (menuPausaAtivo) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                menuPausaAtivo = false;
                exit(0); // Finaliza o programa se clicar no X
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_p) { // Retorna ao jogo com 'P'
                    paused = false;
                    menuPausaAtivo = false;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (pontoDentroDoRetangulo(x, y, &botaoContinuar.rect)) {
                    paused = false;
                    menuPausaAtivo = false; // Retorna ao jogo ao clicar no botão "Continuar"
                }
            }
        }

        // Renderização do menu de pausa
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fundo preto
        SDL_RenderClear(renderer);

        renderizarTexto(renderer, font, "Jogo Pausado", (SDL_Color){255, 255, 255, 255}, 250, 100);
        renderizarBotao(renderer, &botaoContinuar, font, "Continuar");

        SDL_RenderPresent(renderer);
    }
}

// Controle do estado de pausa
void verificarPausa(SDL_Renderer *renderer, TTF_Font *font) {
    if (paused) {
        mostrarMenuPausa(renderer, font);
    }
}
