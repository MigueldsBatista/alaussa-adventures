#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include "map.h"

#define MAX_NAME_LENGTH 50  // Definição do tamanho máximo para o nome do jogador

char playerName[MAX_NAME_LENGTH];  // Definição da variável global

// Definição de cores
SDL_Color corPreta = {0, 0, 0, 255};
SDL_Color corBranca = {255, 255, 255, 255};
SDL_Color corBotao = {0, 255, 0, 255};
SDL_Color corVermelha = {255, 0, 0, 255};
SDL_Color corAmarela = {255, 255, 0, 255};
SDL_Color corAzul = {0, 0, 255, 255};

// Função para renderizar um botão
void renderizarBotao(SDL_Renderer *renderer, Botao *botao, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, botao->color.r, botao->color.g, botao->color.b, botao->color.a);
    SDL_RenderFillRect(renderer, &botao->rect);

    SDL_Surface *surface = TTF_RenderText_Solid(font, botao->texto, corPreta);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) return;

    int textWidth, textHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = {
        botao->rect.x + (botao->rect.w - textWidth) / 2,
        botao->rect.y + (botao->rect.h - textHeight) / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}

// Função para capturar o nome do jogador
void capturarNomeJogador(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event event;
    bool capturando = true;
    int cursorPos = 0;
    memset(playerName, 0, sizeof(playerName));

    while (capturando) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                capturando = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    capturando = false;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE && cursorPos > 0) {
                    playerName[--cursorPos] = '\0';
                } else if (cursorPos < MAX_NAME_LENGTH - 1 && event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z) {
                    playerName[cursorPos++] = (char)event.key.keysym.sym;
                    playerName[cursorPos] = '\0';
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect inputFieldRect = {200, 200, 400, 100};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &inputFieldRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &inputFieldRect);

        SDL_Surface *instrucaoSurface = TTF_RenderText_Solid(font, "Digite seu nome e pressione Enter:", corBranca);
        if (!instrucaoSurface) return;
        SDL_Texture *instrucaoTexture = SDL_CreateTextureFromSurface(renderer, instrucaoSurface);
        SDL_FreeSurface(instrucaoSurface);
        if (!instrucaoTexture) return;

        int instrucaoWidth, instrucaoHeight;
        SDL_QueryTexture(instrucaoTexture, NULL, NULL, &instrucaoWidth, &instrucaoHeight);
        SDL_Rect instrucaoRect = {200, 150, instrucaoWidth, instrucaoHeight};
        SDL_RenderCopy(renderer, instrucaoTexture, NULL, &instrucaoRect);
        SDL_DestroyTexture(instrucaoTexture);

        SDL_Surface *surface = TTF_RenderText_Solid(font, playerName, corBranca);
        if (!surface) return;
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) return;

        int textWidth, textHeight;
        SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect textRect = {
            inputFieldRect.x + (inputFieldRect.w - textWidth) / 2,
            inputFieldRect.y + (inputFieldRect.h - textHeight) / 2,
            textWidth, textHeight
        };

        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);

        SDL_RenderPresent(renderer);
    }
}

// Função para mostrar o menu
bool mostrarMenu(SDL_Renderer *renderer, TTF_Font *font) {
    extern bool noMenu;
    extern bool running;
    extern SDL_Event event;

    int botaoLargura = 200, botaoAltura = 50;

    Botao botaoJogar = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT - 100, botaoLargura, botaoAltura},
        corBotao, "Jogar"
    };

    Botao botaoInstrucoes = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT, botaoLargura, botaoAltura},
        corAmarela, "Comandos"
    };

    Botao botaoSair = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT + 100, botaoLargura, botaoAltura},
        corVermelha, "Sair"
    };

    while (noMenu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                noMenu = false;
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (SDL_PointInRect(&(SDL_Point){x, y}, &botaoJogar.rect)) {
                    capturarNomeJogador(renderer, font);
                    noMenu = false;
                } else if (SDL_PointInRect(&(SDL_Point){x, y}, &botaoInstrucoes.rect)) {
                    mostrarInstrucoes(renderer, font);
                } else if (SDL_PointInRect(&(SDL_Point){x, y}, &botaoSair.rect)) {
                    noMenu = false;
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderizarBotao(renderer, &botaoJogar, font);
        renderizarBotao(renderer, &botaoInstrucoes, font);
        renderizarBotao(renderer, &botaoSair, font);

        SDL_RenderPresent(renderer);
    }

    return noMenu;
}

// Função para renderizar texto
void renderizarTexto(SDL_Renderer *renderer, TTF_Font *font, Texto *texto) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, texto->texto, texto->cor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int largura = surface->w;
    int altura = surface->h;
    SDL_Rect dstRect = {texto->posicao.x, texto->posicao.y, largura, altura};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Função para mostrar as instruções
void mostrarInstrucoes(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_RenderClear(renderer);

    // Posição inicial para o texto das instruções
    SDL_Point posicao = {(SCREEN_WIDTH - 300) / 2, 50};  // Centraliza horizontalmente e define a posição vertical inicial
    int deslocamentoY = 50;  // Espaçamento entre as linhas de instruções

    // Texto das instruções

    // Renderiza os textos
    Texto textoInstrucoes = {posicao, corPreta, "Comandos"};
    renderizarTexto(renderer, font, &textoInstrucoes);
    posicao.y += deslocamentoY;

    Texto textoMoverPersonagem = {posicao, corPreta, "Mova o personagem com 'W, A, S, D'"};
    renderizarTexto(renderer, font, &textoMoverPersonagem);
    posicao.y += deslocamentoY;

    Texto textoPular = {posicao, corPreta, "Para pular, use 'W' ou 'Espaco'"};
    renderizarTexto(renderer, font, &textoPular);
    posicao.y += deslocamentoY;

    Texto textoSair = {posicao, corPreta, "Para sair, clique em 'Sair' no menu"};
    renderizarTexto(renderer, font, &textoSair);
    posicao.y += deslocamentoY;

    Texto textoVoltar = {posicao, corPreta, "Pressione 'ESC' para voltar ao menu"};
    renderizarTexto(renderer, font, &textoVoltar);
    posicao.y += deslocamentoY;

    Texto textoPausar = {posicao, corPreta, "Pressione 'P' para pausar o jogo"};
    renderizarTexto(renderer, font, &textoPausar);

    SDL_RenderPresent(renderer);

    bool instrucoesAtivas = true;
    SDL_Event event;

    // Espera pela interação do usuário
    while (instrucoesAtivas) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                instrucoesAtivas = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    instrucoesAtivas = false;  // Fecha as instruções quando pressionado ESC
                }
            }
        }
    }
}


// Função para mostrar a tela de game over
void showGameOverScreen(SDL_Renderer *renderer, TTF_Font *font) {
    extern bool running;
    bool gameOver = true;

    // Calculando as posições centralizadas do botão
    int botaoLargura = 200;
    int botaoAltura = 50;

    // Botão reiniciar centralizado
    Botao botaoRestart = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT, botaoLargura, botaoAltura},
        corAzul, "Reiniciar"
    };

    while (gameOver) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameOver = false;
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (SDL_PointInRect(&(SDL_Point){x, y}, &botaoRestart.rect)) {
                    gameOver = false;
                    running = true;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);

        // Botão centralizado com texto "Game Over"
        Botao botaoFinish = {
            {CENTER_WIDTH - 100, CENTER_HEIGHT - 100, 200, 50},
            corVermelha,  // Cor Branca
            "Game Over"
        };
        renderizarBotao(renderer, &botaoFinish, font);

        // Renderizar o botão
        renderizarBotao(renderer, &botaoRestart, font);

        SDL_RenderPresent(renderer);
    }
}
// Função para mostrar o menu de pausa
void mostrarMenuPausa(SDL_Renderer *renderer, TTF_Font *font) {
    extern bool paused;

    // Calculando a posição centralizada do botão
    int botaoLargura = 200;
    int botaoAltura = 50;

    // Botão continuar centralizado
    Botao botaoContinuar = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT, botaoLargura, botaoAltura},
        corAzul, "Continuar"
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Texto centralizado
    Botao botaoPausado = {
        {CENTER_WIDTH - 100, CENTER_HEIGHT - 100, 200, 50},
        corVermelha,
        "Jogo Pausado"
    };
    renderizarBotao(renderer, &botaoPausado, font);

    // Renderizar o botão
    renderizarBotao(renderer, &botaoContinuar, font);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
            paused = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (SDL_PointInRect(&(SDL_Point){x, y}, &botaoContinuar.rect)) {
                paused = false;
            }
        }
    }
}

void mostrarMenuFimDeJogo(SDL_Renderer *renderer, TTF_Font *font) {
    extern bool running;
    bool gameOver = true;
    extern bool noMenu;
    // Calculando as posições centralizadas do botão
    int botaoLargura = 200;
    int botaoAltura = 50;

    // Botão reiniciar centralizado
    Botao botaoRestart = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT, botaoLargura, botaoAltura},
        {255, 0, 0, 255}, "Sair"
    };

    while (gameOver) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameOver = false;
                running = false;  // Encerra o jogo
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (SDL_PointInRect(&(SDL_Point){x, y}, &botaoRestart.rect)) {
                    gameOver = false;
                    running = true; // Reinicia o jogo
                    noMenu = true;  // Volta para o menu principal
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Texto centralizado
        Botao botaoFinish = {
            {CENTER_WIDTH - 100, CENTER_HEIGHT - 100, 200, 50},
            {255, 255, 0, 255},  // Cor Amarela
            "Fim de Jogo"
        };
        renderizarBotao(renderer, &botaoFinish, font);
        // Renderizar o botão
        renderizarBotao(renderer, &botaoRestart, font);

        SDL_RenderPresent(renderer);
    }
}
