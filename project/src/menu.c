#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include "map.h"
#include "game.h"

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

        // Renderização
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Campo de entrada centralizado
        SDL_Rect inputFieldRect = {
            (SCREEN_WIDTH - 400) / 2,  // Centraliza horizontalmente
            (SCREEN_HEIGHT - 100) / 2,  // Centraliza verticalmente
            400, 100
        };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &inputFieldRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &inputFieldRect);

        // Texto de instrução centralizado
        SDL_Surface *instrucaoSurface = TTF_RenderText_Solid(font, "Digite seu nome e pressione Enter:", corBranca);
        if (instrucaoSurface) {
            SDL_Texture *instrucaoTexture = SDL_CreateTextureFromSurface(renderer, instrucaoSurface);
            SDL_FreeSurface(instrucaoSurface);
            if (instrucaoTexture) {
                int instrucaoWidth, instrucaoHeight;
                SDL_QueryTexture(instrucaoTexture, NULL, NULL, &instrucaoWidth, &instrucaoHeight);
                SDL_Rect instrucaoRect = {
                    (SCREEN_WIDTH - instrucaoWidth) / 2,  // Centraliza horizontalmente
                    (SCREEN_HEIGHT - 100) / 2 - 50,       // Posiciona um pouco acima do campo de entrada
                    instrucaoWidth,
                    instrucaoHeight
                };
                SDL_RenderCopy(renderer, instrucaoTexture, NULL, &instrucaoRect);
                SDL_DestroyTexture(instrucaoTexture);
            }
        }

        // Renderizar o texto digitado
        SDL_Surface *surface = TTF_RenderText_Solid(font, playerName, corBranca);
        if (surface) {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if (texture) {
                int textWidth, textHeight;
                SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
                SDL_Rect textRect = {
                    inputFieldRect.x + (inputFieldRect.w - textWidth) / 2,
                    inputFieldRect.y + (inputFieldRect.h - textHeight) / 2,
                    textWidth, textHeight
                };
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
        }

        SDL_RenderPresent(renderer);
    }
}



bool mostrarMenu(SDL_Renderer *renderer, TTF_Font *font) {
    extern bool noMenu;
    extern bool running;
    extern SDL_Event event;

    int botaoLargura = 200, botaoAltura = 50;

    // Carrega a fonte para o título com tamanho grande
    TTF_Font *tituloFont = TTF_OpenFont("project/assets/fontes/Open-Sans.ttf", 80);
    if (!tituloFont) {
        printf("Erro ao carregar a fonte do título: %s\n", TTF_GetError());
        return false; // Retorna false se a fonte do título não puder ser carregada
    }

    // Título do jogo
    const char *titulo = "Super Mario Inspired Game";
    SDL_Color corTitulo = {255, 255, 255, 255};  // Cor branca para o título

    // Renderiza o título em uma superfície
    SDL_Surface *tituloSurface = TTF_RenderText_Solid(tituloFont, titulo, corTitulo);
    if (!tituloSurface) {
        printf("Erro ao criar a superfície do título: %s\n", TTF_GetError());
        TTF_CloseFont(tituloFont);
        return false;
    }

    // Cria uma textura a partir da superfície do título
    SDL_Texture *tituloTexture = SDL_CreateTextureFromSurface(renderer, tituloSurface);
    if (!tituloTexture) {
        printf("Erro ao criar a textura do título: %s\n", SDL_GetError());
        SDL_FreeSurface(tituloSurface);
        TTF_CloseFont(tituloFont);
        return false;
    }

    int tituloLargura, tituloAltura;
    SDL_QueryTexture(tituloTexture, NULL, NULL, &tituloLargura, &tituloAltura);

    SDL_Rect tituloRect = {
        CENTER_WIDTH - tituloLargura / 2,
        50,  // Posição no topo da tela
        tituloLargura,
        tituloAltura
    };

    Botao botaoJogar = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT - 100, botaoLargura, botaoAltura},
        corBotao, "Jogar"
    };

    Botao botaoInstrucoes = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT, botaoLargura, botaoAltura},
        corAmarela, "Comandos"
    };

    Botao botaoRanking = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT + 100, botaoLargura, botaoAltura},
        corAzul, "Ranking"
    };

    Botao botaoSair = {
        {CENTER_WIDTH - botaoLargura / 2, CENTER_HEIGHT + 200, botaoLargura, botaoAltura},
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
                } else if(SDL_PointInRect(&(SDL_Point){x, y}, &botaoRanking.rect)){
                    mostrarRanking(renderer,font);
                } else if (SDL_PointInRect(&(SDL_Point){x, y}, &botaoSair.rect)) {
                    noMenu = false;
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Renderiza o título
        SDL_RenderCopy(renderer, tituloTexture, NULL, &tituloRect);

        renderizarBotao(renderer, &botaoJogar, font);
        renderizarBotao(renderer, &botaoInstrucoes, font);
        renderizarBotao(renderer, &botaoRanking, font);
        renderizarBotao(renderer, &botaoSair, font);

        SDL_RenderPresent(renderer);
    }

    // Libera recursos do título
    SDL_DestroyTexture(tituloTexture);
    SDL_FreeSurface(tituloSurface);
    TTF_CloseFont(tituloFont);

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
    extern bool noMenu;
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
                    noMenu=true;
                    shutdownGame(); // Limpa a memória alocada
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

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
                    shutdownGame(); // Limpa a memória alocada
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

void mostrarRanking(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_RenderClear(renderer);

    FILE *file = fopen("ranking.txt", "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de ranking.\n");
        return;
    }

    char linha[256];
    int deslocamentoY = 30;  // Espaçamento entre as linhas do ranking
    int cont = 0;

    // Estrutura para armazenar as informações dos jogadores
    typedef struct {
        char nome[50];
        int pontos;
        char dataHora[100];
    } Jogador;

    Jogador jogadores[100];
    int numJogadores = 0;

    // Lê as informações do arquivo e armazena na estrutura
    while (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "Jogador: %[^|]| Pontos: %d | Data e Hora: %[^\n]", jogadores[numJogadores].nome, &jogadores[numJogadores].pontos, jogadores[numJogadores].dataHora);
        numJogadores++;
    }

    // Ordena os jogadores por pontuação (maior para menor)
    for (int i = 0; i < numJogadores - 1; i++) {
        for (int j = i + 1; j < numJogadores; j++) {
            if (jogadores[i].pontos < jogadores[j].pontos) {
                Jogador temp = jogadores[i];
                jogadores[i] = jogadores[j];
                jogadores[j] = temp;
            }
        }
    }

    int posicaoRanking = 1;
    SDL_Color corBranca = {255, 255, 255, 255};
    SDL_Point posicaoInicial = {0, 50};  // Posição inicial para o texto do ranking, centralizado

    // Renderiza os jogadores ordenados e centralizados
    for (int i = 0; i < numJogadores && i < 10; i++) {
        char texto[256];
        snprintf(texto, sizeof(texto), " %d  Jogador: %s | Pontos: %d | Data e Hora: %s", posicaoRanking, jogadores[i].nome, jogadores[i].pontos, jogadores[i].dataHora);
        
        int textoWidth, textoHeight;
        TTF_SizeText(font, texto, &textoWidth, &textoHeight);

        SDL_Point posicaoCentralizada = {
            (SCREEN_WIDTH - textoWidth) / 2,  // Centraliza horizontalmente
            posicaoInicial.y + i * deslocamentoY  // Ajusta a posição vertical com base no índice
        };

        Texto textoRanking = {posicaoCentralizada, corBranca, texto};
        renderizarTexto(renderer, font, &textoRanking);
        
        posicaoRanking++;
    }

    fclose(file);
    SDL_RenderPresent(renderer);

    bool rankingAtivo = true;
    SDL_Event event;

    // Espera pela interação do usuário
    while (rankingAtivo) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                rankingAtivo = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    rankingAtivo = false;  // Fecha o ranking quando pressionado ESC
                }
            }
        }
    }
}
