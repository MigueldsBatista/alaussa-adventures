#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "map.h"
#include "entity.h"
#include "enemy.h"
#include "sprite.h"
#include <SDL2/SDL_ttf.h>
#include "menu.h"
const float escala = 0.5; // Ajuste conforme a proporção da tela minimizada

Map gameMap;

void loadMap(const char* map_file) {
    //TODO: Implementar a função loadMap
    FILE* file = fopen(map_file, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo do mapa: %s\n", map_file);
        return;
    }

    fscanf(file, "%d %d", &gameMap.width, &gameMap.height);
    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            fscanf(file, "%d", &gameMap.tiles[y][x]); // Lê cada tile do mapa
        }
    }

    fclose(file);
}
// Example of a map file content:
// 5 5
// 1 1 1 1 1
// 1 0 0 0 1
// 1 0 1 0 1
// 1 0 0 0 1
// 1 1 1 1 1

// The first line contains the width and height of the map.
// The following lines contain the tile values for each row of the map.
static int first_time_render = 0;
static SDL_Texture* bloco_texture;
static SDL_Texture* brick_texture;
static SDL_Texture* coin_texture;
static SDL_Texture* torreFirst;
static SDL_Texture* torreSecond;
static SDL_Texture* torreThird;

void renderMap(SDL_Renderer* renderer) {
    if (first_time_render == 0) {
        first_time_render = 1;
        bloco_texture = loadTexture("project/assets/blocks/block.png", renderer, "bloco");
        brick_texture = loadTexture("project/assets/blocks/brick_game.png", renderer, "brick");
        coin_texture = loadTexture("project/assets/blocks/coin.png", renderer, "coin");
        torreFirst = loadTexture("project/assets/map/torre_0.png", renderer, "torre");
        torreSecond = loadTexture("project/assets/map/torre_1.png", renderer, "torre");
        torreThird = loadTexture("project/assets/map/torre_2.png", renderer, "torre");
    }

    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            int tile = gameMap.tiles[y][x];
            SDL_Rect dst_rect = { x * 64, y * 64, 64, 64 };
            SDL_Texture *rendered_texture = NULL;

            // Seleciona a textura com base no valor do tile
            if (tile == 1) {
                rendered_texture = brick_texture;
                rendered_texture = bloco_texture;
            } else if (tile == 2) {
                addEnemy(x * 64, y * 64, renderer);
                gameMap.tiles[y][x] = 0; // Marca o tile como vazio para evitar adição duplicada
            } else if (tile == 3) {
                rendered_texture = coin_texture;
            } else if (tile == 4) {
                rendered_texture = torreThird;
            } else if (tile == 5) {
                rendered_texture = torreSecond;
            } else if (tile == 6) {
                rendered_texture = torreFirst;
            }

            // Executa SDL_RenderCopy apenas uma vez se houver uma textura a ser renderizada
            if (rendered_texture != NULL) {
                SDL_RenderCopy(renderer, rendered_texture, NULL, &dst_rect);
            }
        }
    }
}

void renderBackground(SDL_Renderer* renderer, SDL_Texture* background, int screen_width, int screen_height) {
    // Renderiza a imagem de fundo sem efeito de parallax
    SDL_Rect src_rect = { 0, 0, screen_width, screen_height };
    SDL_Rect dst_rect = { 0, 0, screen_width, screen_height };
    SDL_RenderCopy(renderer, background, &src_rect, &dst_rect);
}


// Função para verificar colisões entre o jogador e blocos
bool checkEntityBlockCollision(Entity *player) {
    SDL_Rect playerRect = { 
        (int)(player->position.x * escala), 
        (int)(player->position.y * escala), 
        (int)(player->width * escala), 
        (int)(player->height * escala) 
    };

    player->position.onGround = false;  // Assume que o jogador está no ar até encontrar um bloco embaixo

    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            if (gameMap.tiles[y][x] > 0 && gameMap.tiles[y][x]!= 3 && gameMap.tiles[y][x]!=4 && gameMap.tiles[y][x]!= 5 && gameMap.tiles[y][x]!= 6 ) {  // Somente checar blocos sólidos
                SDL_Rect blockRect = { 
                    (int)(x * 64 * escala), 
                    (int)(y * 64 * escala), 
                    (int)(64 * escala), 
                    (int)(64 * escala) 
                };
            
                if (SDL_HasIntersection(&playerRect, &blockRect)==SDL_TRUE) {
                    // Colisão por cima (o jogador aterrissa no bloco)
                    if (playerRect.y + playerRect.h <= blockRect.y + 5 && player->position.velY >= 0) {
                        player->position.onGround = true;
                        player->position.y = blockRect.y / escala - player->height;  // Define a posição exata em cima do bloco
                        player->position.velY = 0;  // Zera a velocidade vertical
                        return true;
                    }
                    // Colisão por baixo (o jogador bate a cabeça no bloco)
                    else if (playerRect.y >= blockRect.y + blockRect.h && player->position.velY < 0) {
                        player->position.y = (blockRect.y + blockRect.h) / escala;
                        player->position.velY = 0;  // Zera a velocidade vertical ao colidir por baixo
                        return true;
                    }

                    // Colisão lateral (ajustes de posição horizontal)
                    else if (playerRect.x + playerRect.w > blockRect.x && playerRect.x < blockRect.x) {
                        player->position.x = blockRect.x / escala - player->width;  // Ajuste suave pela esquerda
                        player->position.velX = 0;  // Zera a velocidade horizontal
                    }

                    else if (playerRect.x < blockRect.x + blockRect.w && playerRect.x > blockRect.x) {
                        player->position.x = (blockRect.x + blockRect.w) / escala;  // Ajuste suave pela direita
                        player->position.velX = 0;  // Zera a velocidade horizontal
                    }
                }
            }
        }
    }
    return false;
}


void checkMapTransition(Entity *player, SDL_Renderer *renderer) {
static int currentMap = 0;
    char mapFile[256];

    if (player->position.x + player->width >= SCREEN_WIDTH) {
        currentMap++;
        freeEnemyList();
        snprintf(mapFile, sizeof(mapFile), "project/assets/map/map_%d.txt", currentMap); // Formata o nome do próximo mapa
        loadMap(mapFile); // Carrega o próximo mapa
        curarTrocademapa(player);
        player->position.x = 0; // Reseta a posição do jogador para o início do novo mapa
        printf("Mapa %d\n", currentMap);
    } //else if (player->position.x <= 0 && currentMap > 0) {
      //  currentMap--;
      //  snprintf(mapFile, sizeof(mapFile), "project/assets/map/map_%d.txt", currentMap); // Formata o nome do mapa anterior
     //   loadMap(mapFile); // Carrega o mapa anterior
      //  player->position.x = SCREEN_WIDTH - player->width; // Reseta a posição do jogador para o final do mapa anterior
       // freeEnemyList();
    //}
    
    if(player->isAlive==false)currentMap=0;

    renderMap(renderer);
}

void checkCoinCollected(Entity *player, SDL_Renderer *renderer) {
    SDL_Rect playerRect = {
        (int)player->position.x,
        (int)player->position.y,
        player->width,
        player->height
    };

    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            if (gameMap.tiles[y][x] == 3) {
                SDL_Rect coinRect = {
                    x * 64,
                    y * 64,
                    64,
                    64
                };

                if (SDL_HasIntersection(&playerRect, &coinRect)) {
                    player->moedas++;
                    if (player->moedas % 5 == 0) { 
                        Life *novaVida = (Life *)malloc(sizeof(Life));
                        if (!novaVida) {
                            printf("Erro ao alocar memória para nova vida\n");
                            return;
                        }
                        if (player->head != NULL) {
                            novaVida->id = player->head->id + 1;
                        } else {
                            novaVida->id = 1;  
                        }
                        novaVida->prox = player->head;
                        player->head = novaVida;
                    }
                    gameMap.tiles[y][x] = 0;  // Remove a moeda do mapa
                }
            }
        }
    }
}


bool checkPlayerInFinishPosition(Entity *player, SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Rect playerRect = {
        (int)player->position.x,
        (int)player->position.y,
        player->width,
        player->height
    };
     
    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            if (gameMap.tiles[y][x] == 4 || gameMap.tiles[y][x] == 5 || gameMap.tiles[y][x] == 6) {
                SDL_Rect finishRect = {
                    x * 64,
                    y * 64,
                    64,
                    64
                };

                if (SDL_HasIntersection(&playerRect, &finishRect)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void updateEnemyMovement(Entity *enemy) {
    SDL_Rect enemyRect = {
        (int)(enemy->position.x * escala),
        (int)(enemy->position.y * escala),
        (int)(enemy->width * escala),
        (int)(enemy->height * escala)
    };

    // Verificar colisão lateral
    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            if (gameMap.tiles[y][x] > 0 && gameMap.tiles[y][x] != 3) {
                SDL_Rect blockRect = {
                    (int)(x * 64 * escala),
                    (int)(y * 64 * escala),
                    (int)(64 * escala),
                    (int)(64 * escala)
                };

                if (SDL_HasIntersection(&enemyRect, &blockRect) == SDL_TRUE) {
                    // Colisão lateral com um bloco, inverte a direção
                    if (enemy->position.velX > 0) {
                        enemy->position.x = blockRect.x / escala - enemy->width;
                    } else {
                        enemy->position.x = (blockRect.x + blockRect.w) / escala;
                    }
                    enemy->position.velX *= -1; // Inverte a direção
                }
            }
        }
    }

    // Verificar borda da plataforma
    int tileBelowLeft = gameMap.tiles[(int)((enemy->position.y + enemy->height + 1) / 64)][(int)(enemy->position.x / 64)];
    int tileBelowRight = gameMap.tiles[(int)((enemy->position.y + enemy->height + 1) / 64)][(int)((enemy->position.x + enemy->width) / 64)];

    // Inverte a direção se o inimigo estiver na borda da plataforma
    if (tileBelowLeft == 0 && enemy->position.velX < 0) {
        enemy->position.velX *= -1;
    } else if (tileBelowRight == 0 && enemy->position.velX > 0) {
        enemy->position.velX *= -1;
    }

    // Atualizar a posição do inimigo
    enemy->position.x += enemy->position.velX;
}

void checkPlatformEdge(Entity *enemy) {
    // Cálculo do tile abaixo da borda esquerda do inimigo
    int tileBelowLeft =
     gameMap.tiles[(int)((enemy->position.y + enemy->height + 1) / 64)] // Calcula a linha do tile
    [(int)(enemy->position.x / 64)]; // Calcula a coluna do tile

    // Cálculo do tile abaixo da borda direita do inimigo
    int tileBelowRight = gameMap.tiles[(int)((enemy->position.y + enemy->height + 1) / 64)] // Calcula a linha do tile
                         [(int)((enemy->position.x + enemy->width) / 64)]; // Calcula a coluna do tile

    // Verifica se não há plataforma à esquerda e o inimigo está se movendo para a esquerda
    if (tileBelowLeft == 0 && enemy->position.velX < 0) {
        // Inverte a direção horizontal do inimigo (movimento para a direita)
        enemy->position.velX *= -1;
    } 
    // Verifica se não há plataforma à direita e o inimigo está se movendo para a direita
    else if (tileBelowRight == 0 && enemy->position.velX > 0) {
        // Inverte a direção horizontal do inimigo (movimento para a esquerda)
        enemy->position.velX *= -1;
    }

}