#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"

// ver se a pessoa clicou no botão
bool pontoDentroDoRetangulo(int x, int y, SDL_Rect *rect) {
    return x >= rect->x && x <= rect->x + rect->w &&
           y >= rect->y && y <= rect->y + rect->h;
}

//função de criar botão
void renderizarBotao(SDL_Renderer *renderer, Botao *botao, TTF_Font *font, const char *text) {
    // Cria o botão, so o retangulo
    SDL_SetRenderDrawColor(renderer, botao->color.r, botao->color.g, botao->color.b, botao->color.a);
    SDL_RenderFillRect(renderer, &botao->rect);

    // Cria o texto dentro do botão
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, (SDL_Color){0, 0, 0});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int text_width = surface->w;
    int text_height = surface->h;

    SDL_Rect textRect = {
        botao->rect.x + (botao->rect.w - text_width) / 2,
        botao->rect.y + (botao->rect.h - text_height) / 2, 
        text_width,
        text_height
    };

    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Colocar os textos das instruções após apertar o botão de "como joga?"
void renderizarTexto(SDL_Renderer *renderer, TTF_Font *font, const char *texto, SDL_Color cor, int x, int y) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, texto, cor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int largura = surface->w;
    int altura = surface->h;

    SDL_Rect dstrect = {x, y, largura, altura};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void mostrarInstrucoes(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color corTexto = {0, 0, 0, 0};
    SDL_RenderClear(renderer);

    renderizarTexto(renderer, font, "Instrucoes:", corTexto, 250, 50);
    renderizarTexto(renderer, font, "mova o personagem no 'w,a,s,d'", corTexto, 10, 100);
    renderizarTexto(renderer, font, "para pular pode ser tanto no W quanto no espaco", corTexto, 10, 150);
    renderizarTexto(renderer, font, "se quiser sair clique na tela anterior 'sair'", corTexto, 10, 200);
    renderizarTexto(renderer, font, "Pressione 'esc' para voltar ao menu.", corTexto, 10, 250);
    renderizarTexto(renderer, font, "Pressione 'P' para pausar o jogo.", corTexto, 10, 300);

    SDL_RenderPresent(renderer);

    // espera ação pra atualizar a tela
    bool instrucoesAtivas = true;
    SDL_Event event;
    while (instrucoesAtivas) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                instrucoesAtivas = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) { // Volta ao menu com 'esc'
                    instrucoesAtivas = false;
                }
            }
        }
    }
}