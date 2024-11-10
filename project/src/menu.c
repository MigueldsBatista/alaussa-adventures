#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include "map.h"

char playerName[50];  // Definição da variável global

SDL_Color corPreta = {0, 0, 0, 255};// Cor do texto é preta
SDL_Color corBranca = {255, 255, 255, 255};// Cor do texto é branca

SDL_Color corBotao ={0, 255, 0, 255};
SDL_Color corVermelha = {255, 0, 0, 255};
SDL_Color corAmarela = {255, 255, 0, 255};
SDL_Color corAzul = {0, 0, 255, 255};
/// Função para renderizar um botão
void renderizarBotao(SDL_Renderer *renderer, Botao *botao, TTF_Font *font) {
    // Criação do retângulo do botão
    SDL_SetRenderDrawColor(renderer, botao->color.r, botao->color.g, botao->color.b, botao->color.a);
    SDL_RenderFillRect(renderer, &botao->rect);

    // Renderizar o texto no botão
    SDL_Surface *surface = TTF_RenderText_Solid(font, botao->texto, (SDL_Color){0, 0, 0});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_Rect textRect = {
        botao->rect.x + (botao->rect.w - textWidth) / 2,
        botao->rect.y + (botao->rect.h - textHeight) / 2,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Função para mostrar o menu
char playerName[MAX_NAME_LENGTH];

// Função para capturar o nome do jogador
void capturarNomeJogador(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event event;
    bool capturando = true;
    int cursorPos = 0;
    memset(playerName, 0, sizeof(playerName));  // Limpa o nome anterior

    while (capturando) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                capturando = false;  // Fecha o loop em caso de quit
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    capturando = false;  // Finaliza a captura com Enter
                } else if (event.key.keysym.sym == SDLK_BACKSPACE && cursorPos > 0) {
                    playerName[--cursorPos] = '\0';  // Apaga o último caractere
                } else if (cursorPos < MAX_NAME_LENGTH - 1) {  // Impede exceder o limite de tamanho
                    playerName[cursorPos++] = (char)event.key.keysym.sym;  // Adiciona o caractere
                    playerName[cursorPos] = '\0';  // Garante a terminação da string
                }
            }
        }

        // Limpa a tela e renderiza o nome digitado
        SDL_RenderClear(renderer);
        
        SDL_Color corTexto = {255, 255, 255, 255};  // Cor branca para o texto
        SDL_Surface *surface = TTF_RenderText_Solid(font, playerName, corTexto);
        if (surface == NULL) {
            printf("Erro ao criar a superfície do texto: %s\n", TTF_GetError());
            continue;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL) {
            printf("Erro ao criar a textura do texto: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            continue;
        }

        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_Rect textRect = {
            (800 - textWidth) / 2,  // Centraliza horizontalmente na tela
            100,                    // Posição vertical fixa
            textWidth, textHeight
        };

        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_RenderPresent(renderer);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}


bool mostrarMenu(SDL_Renderer *renderer, TTF_Font *font) {
    extern bool noMenu;
    extern bool running;
    extern SDL_Event event;

    // Calculando a posição do botão centralizado
    int botaoLargura = 200;
    int botaoAltura = 50;

    // Botões centralizados
    Botao botaoJogar = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT - 100, botaoLargura, botaoAltura},
        {0, 255, 0, 255}, 
        "Jogar"
    };

    Botao botaoInstrucoes = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT, botaoLargura, botaoAltura},
        {255, 255, 0, 255}, 
        "Comandos"
    };

    Botao botaoSair = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT + 100, botaoLargura, botaoAltura},
        {255, 0, 0, 255}, 
        "Sair"
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
        {255, 0, 0, 255}, "Reiniciar"
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
        SDL_RenderClear(renderer);

        // Texto centralizado
        Texto textoFinish = {
            (SDL_Point){CENTER_WIDTH - 100, CENTER_HEIGHT - 100}, {255, 255, 255, 255}, "Game Over"};
        renderizarTexto(renderer, font, &textoFinish);

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
