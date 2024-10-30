#include <SDL2/SDL_image.h>
#include "menu.h"



// Função para verificar se um ponto está dentro de um retângulo (utilizada para clique)
bool pontoDentroDoRetangulo(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && x <= rect->x + rect->w &&
           y >= rect->y && y <= rect->y + rect->h;
}

// Função para renderizar um botão
void renderizarBotao(SDL_Renderer *renderer, Botao *botao) {
    SDL_SetRenderDrawColor(renderer, botao->color.r, botao->color.g, botao->color.b, botao->color.a);
    SDL_RenderFillRect(renderer, &botao->rect);
}
