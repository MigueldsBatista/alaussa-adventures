#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"


// Função para exibir o menu
bool mostrarMenu(SDL_Renderer* renderer, TTF_Font* font) {
    extern bool noMenu;
    extern bool running;
    extern SDL_Event event;
    Botao botaoJogar = {{230, 100, 200, 50}, {0, 255, 0, 255}};
    Botao botaoInstrucoes = {{230, 200, 200, 50}, {255, 255, 0, 255}};
    Botao botaoSair = {{230, 300, 200, 50}, {255, 0, 0, 255}};

    while (noMenu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                noMenu = false;
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (pontoDentroDoRetangulo(x, y, &botaoJogar.rect)) {
                    noMenu = false;
                } else if (pontoDentroDoRetangulo(x, y, &botaoInstrucoes.rect)) {
                    mostrarInstrucoes(renderer, font);
                } else if (pontoDentroDoRetangulo(x, y, &botaoSair.rect)) {
                    noMenu = false;
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        loadMap("./project/assets/map/map_0.txt");
        renderizarBotao(renderer, &botaoJogar, font, "Jogar");
        renderizarBotao(renderer, &botaoInstrucoes, font, "Comandos");
        renderizarBotao(renderer, &botaoSair, font, "Sair");
        SDL_RenderPresent(renderer);
    }

    return noMenu;
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

void showGameOverScreen(SDL_Renderer* renderer, TTF_Font* font) {
    bool gameOver = true;
    extern bool running;
    Botao botaoRestart = {{230, 300, 200, 50}, {255, 0, 0, 255}};
    
    while (gameOver) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {// Fecha o jogo se clicar no X que é o codigo numero 256
                gameOver = false;
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                SDL_Point ponto = {x, y};
                if (SDL_PointInRect(&ponto, &botaoRestart.rect)) {
                    gameOver = false;  // Restart the game
                    running = true;
                }
            }
        }

        // Renderização da tela de game over
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fundo preto
        SDL_RenderClear(renderer);

        renderizarTexto(renderer, font, "Game Over", (SDL_Color){255, 255, 255, 255}, 250, 100);
        renderizarBotao(renderer, &botaoRestart, font, "Restart");

        SDL_RenderPresent(renderer);
    }
}