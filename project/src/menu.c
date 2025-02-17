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
        return false;
    }
    TTF_Font *subTituloFont = TTF_OpenFont("project/assets/fontes/Open-Sans.ttf", 40);
    if (!subTituloFont) {
        printf("Erro ao carregar a fonte do título: %s\n", TTF_GetError());
        return false; 
    }

    // Título do jogo
    const char *titulo = "Alaursa adventures";
    SDL_Color corTitulo = {0, 0, 0, 0}; 
    
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

    SDL_Rect tituloRect = {250, 100, tituloLargura,tituloAltura};

    Botao botaoJogar = {
        {550, CENTER_HEIGHT - 150, botaoLargura, botaoAltura},
        corBotao, "Jogar"
    };

    Botao botaoInstrucoes = {
        {550, CENTER_HEIGHT - 75, botaoLargura, botaoAltura},
        corAmarela, "Comandos"
    };

    Botao botaoRanking = {
        {550, CENTER_HEIGHT, botaoLargura, botaoAltura},
        corAzul, "Ranking"
    };

    Botao botaoSair = {
        {550, CENTER_HEIGHT+75, botaoLargura, botaoAltura},
        corVermelha, "Sair"
    };

     // Carrega a textura de fundo
    SDL_Surface *fundoSurface = IMG_Load("project/assets/images/hihihiha.png");
    if (!fundoSurface) {
        printf("Erro ao carregar a imagem de fundo: %s\n", IMG_GetError());
        return false;
    }
    SDL_Texture *fundoTexture = SDL_CreateTextureFromSurface(renderer, fundoSurface);
    SDL_FreeSurface(fundoSurface);

    if (!fundoTexture) {
        printf("Erro ao criar a textura de fundo: %s\n", SDL_GetError());
        return false;
    }

    while (noMenu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                noMenu = false;
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (SDL_PointInRect(&(SDL_Point){x, y}, &botaoJogar.rect)) {
                    capturarNomeJogador1(renderer, font);
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
        // Renderiza o fundo
        SDL_RenderCopy(renderer, fundoTexture, NULL, NULL);
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
    SDL_Surface *surface = TTF_RenderText_Solid(font, texto->text, texto->cor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int largura = surface->w;
    int altura = surface->h;
    SDL_Rect dstRect = {texto->posicao.x, texto->posicao.y, largura, altura};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void mostrarInstrucoes(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_RenderClear(renderer);

    // Posição vertical inicial para o texto das instruções
    int posicaoY = 50;  // Define a posição vertical inicial para o primeiro texto
    int deslocamentoY = 50;  // Espaçamento entre as linhas de instruções

    // Texto das instruções
    const char *instrucoes[] = {
        "Comandos",
        "Mova o personagem com 'W, A, S, D' ou nas setinhas",
        "Para pular, use 'W' ou 'Espaco'",
        "Para sair, clique em 'Sair' no menu",
        "Pressione 'P' para pausar o jogo",
        "Pressione 'ESC' para voltar ao menu"
    };
    SDL_Color corPreta = {255, 255, 255, 255};

    // Renderiza cada linha de texto centralizada
    for (int i = 0; i < 6; i++) {
        // Renderiza o texto para uma superfície temporária para obter suas dimensões
        SDL_Surface *surface = TTF_RenderText_Solid(font, instrucoes[i], corPreta);
        if (!surface) {
            printf("Erro ao criar a superfície de texto: %s\n", TTF_GetError());
            continue;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        int larguraTexto = surface->w;
        SDL_FreeSurface(surface);  // Libera a superfície após criar a textura

        // Define a posição centralizada para o texto
        SDL_Rect textoRect = {
            (SCREEN_WIDTH - larguraTexto) / 2,  // Centraliza horizontalmente
            posicaoY,                           // Posição vertical
            larguraTexto,
            surface->h
        };

        // Renderiza o texto na posição definida
        SDL_RenderCopy(renderer, texture, NULL, &textoRect);
        SDL_DestroyTexture(texture);  // Libera a textura após renderizá-la

        posicaoY += deslocamentoY;  // Atualiza a posição vertical para a próxima linha
    }

    SDL_RenderPresent(renderer);

    // Loop de evento para manter a tela de instruções ativa
    bool instrucoesAtivas = true;
    SDL_Event event;
    while (instrucoesAtivas) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                instrucoesAtivas = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                instrucoesAtivas = false;  // Fecha as instruções quando pressionado ESC
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

// Função para criar um novo jogador
Jogador* criarJogador(const char* nome, int pontos, const char* dataHora) {
    Jogador* novoJogador = (Jogador*)malloc(sizeof(Jogador));
    strcpy(novoJogador->nome, nome);
    novoJogador->pontos = pontos;
    strcpy(novoJogador->dataHora, dataHora);
    novoJogador->prox = NULL;
    return novoJogador;
}

// Função para adicionar um jogador à lista encadeada
void adicionarJogador(Jogador** head, Jogador* novoJogador) {
    if (*head == NULL) {
        *head = novoJogador;
    } else {
        Jogador* temp = *head;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = novoJogador;
    }
}

// Função para ordenar a lista de jogadores por pontuação (maior para menor)
void ordenarLista(Jogador** head) {
    if (*head == NULL) return;

    for (Jogador* i = *head; i != NULL; i = i->prox) {
        for (Jogador* j = i->prox; j != NULL; j = j->prox) {
            if (i->pontos < j->pontos) {
                // Troca as informações dos jogadores
                int tempPontos = i->pontos;
                char tempNome[50], tempDataHora[100];
                strcpy(tempNome, i->nome);
                strcpy(tempDataHora, i->dataHora);

                i->pontos = j->pontos;
                strcpy(i->nome, j->nome);
                strcpy(i->dataHora, j->dataHora);

                j->pontos = tempPontos;
                strcpy(j->nome, tempNome);
                strcpy(j->dataHora, tempDataHora);
            }
        }
    }
}

// Função para liberar a memória da lista encadeada
void liberarLista(Jogador* head) {
    while (head != NULL) {
        Jogador* temp = head;
        head = head->prox;
        free(temp);
    }
}

void capturarNomeJogador1(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event event;
    bool capturando = true;
    int cursorPos = 0;
    memset(playerName, 0, sizeof(playerName));

    SDL_StartTextInput();  // Inicia o modo de entrada de texto

    while (capturando) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                capturando = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                capturando = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && cursorPos > 0) {
                playerName[--cursorPos] = '\0';
            } else if (event.type == SDL_TEXTINPUT && cursorPos < MAX_NAME_LENGTH - 1) {
                strncat(playerName, event.text.text, 1);
                cursorPos++;
            }
        }

        // Renderização (exibição do nome do jogador e instruções)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_Rect inputFieldRect = {
            (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 100) / 2, 400, 100
        };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &inputFieldRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &inputFieldRect);

        // Renderiza as instruções
        SDL_Surface *instrucaoSurface = TTF_RenderText_Solid(font, "Digite seu nome e pressione Enter:", corBranca);
        if (instrucaoSurface) {
            SDL_Texture *instrucaoTexture = SDL_CreateTextureFromSurface(renderer, instrucaoSurface);
            SDL_FreeSurface(instrucaoSurface);
            if (instrucaoTexture) {
                int instrucaoWidth, instrucaoHeight;
                SDL_QueryTexture(instrucaoTexture, NULL, NULL, &instrucaoWidth, &instrucaoHeight);
                SDL_Rect instrucaoRect = {
                    (SCREEN_WIDTH - instrucaoWidth) / 2,
                    (SCREEN_HEIGHT - 100) / 2 - 50,
                    instrucaoWidth,
                    instrucaoHeight
                };
                SDL_RenderCopy(renderer, instrucaoTexture, NULL, &instrucaoRect);
                SDL_DestroyTexture(instrucaoTexture);
            }
        }

        // Renderiza o nome digitado
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

    SDL_StopTextInput();  // Finaliza o modo de entrada de texto
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

    // Lista encadeada para armazenar os jogadores
    Jogador* head = NULL;

    // Lê as informações do arquivo e armazena na lista encadeada
    while (fgets(linha, sizeof(linha), file)) {
        Jogador* novoJogador = (Jogador*)malloc(sizeof(Jogador));
        sscanf(linha, "Jogador: %[^|]| Pontos: %d | Data e Hora: %[^\n]", novoJogador->nome, &novoJogador->pontos, novoJogador->dataHora);
        novoJogador->prox = NULL;
        adicionarJogador(&head, novoJogador);
    }

    // Ordena os jogadores por pontuação
    ordenarLista(&head);

    int posicaoRanking = 1;
    SDL_Color corBranca = {255, 255, 255, 255};
    SDL_Point posicaoInicial = {0, 50};  // Posição inicial para o texto do ranking, centralizado

    // Renderiza os jogadores ordenados e centralizados
    Jogador* atual = head;
    for (int i = 0; i < 10 && atual != NULL; i++) {
        char texto[256];
        snprintf(texto, sizeof(texto), " %d  Jogador: %s | Pontos: %d | Data e Hora: %s", posicaoRanking, atual->nome, atual->pontos, atual->dataHora);

        int textoWidth, textoHeight;
        TTF_SizeText(font, texto, &textoWidth, &textoHeight);

        SDL_Point posicaoCentralizada = {
            (SCREEN_WIDTH - textoWidth) / 2,  // Centraliza horizontalmente
            posicaoInicial.y + i * deslocamentoY  // Ajusta a posição vertical com base no índice
        };

        Texto textoRanking = {posicaoCentralizada, corBranca, texto};
        renderizarTexto(renderer, font, &textoRanking);

        posicaoRanking++;
        atual = atual->prox;
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

    liberarLista(head);
}