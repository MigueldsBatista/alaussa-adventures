#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "map.h"
#include "player.h"
#include "enemy.h"
const float escala = 0.5; // Ajuste conforme a proporção da tela minimizada


// Estrutura de dados para o mapa
typedef struct {
    int width;
    int height;
    int tiles[MAX_MAP_HEIGHT][MAX_MAP_WIDTH]; // Exemplo de representação do mapa
} Map;

Map gameMap; // Instância do mapa

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

// Variável global para a posição da câmera


void updateCameraPosition(Player *player) {
    int camera_position = 0;
    int player_center = player->position.x + (player->width / 2);
    camera_position = player_center - (screen_width / 2);
    if (camera_position < 0) camera_position = 0;  // Limite para evitar câmera negativa
}

void renderMap(SDL_Renderer* renderer, SDL_Texture* bloco_texture) {
    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            int tile = gameMap.tiles[y][x];
            if (tile > 0) {  // Supondo que um tile "0" é vazio
                SDL_Rect dst_rect = { 
                    (x * 64) - camera_position, // Aplica o deslocamento da câmera
                    y * 64, 
                    64, 
                    64 
                };
                SDL_RenderCopy(renderer, bloco_texture, NULL, &dst_rect);
            }
        }
    }
}

void renderBackground(SDL_Renderer* renderer, SDL_Texture* background, int screen_width, int screen_height) {
    int bg_width = 725;  // Largura da imagem de fundo
    int bg_height = 400; // Altura da imagem de fundo

    // Aplica parallax dividindo o movimento da câmera
    int parallax_offset = (camera_position / 2) % bg_width;

    SDL_Rect src_rect1 = { parallax_offset, 0, bg_width - parallax_offset, bg_height };
    SDL_Rect dst_rect1 = { 0, 0, bg_width - parallax_offset, bg_height };
    SDL_RenderCopy(renderer, background, &src_rect1, &dst_rect1);

    if (screen_width > (bg_width - parallax_offset)) {
        SDL_Rect src_rect2 = { 0, 0, parallax_offset, bg_height };
        SDL_Rect dst_rect2 = { bg_width - parallax_offset, 0, parallax_offset, bg_height };
        SDL_RenderCopy(renderer, background, &src_rect2, &dst_rect2);
    }
}


// Function to check for collision between two rectangles
bool checkCollision(SDL_Rect a, SDL_Rect b) {
    // Check if the rectangles overlap
    if (a.x + a.w <= b.x || a.x >= b.x + b.w || a.y + a.h <= b.y || a.y >= b.y + b.h) {
        return false;
    }
    return true;
}

// Função para verificar colisões entre o jogador e blocos
bool checkPlayerBlockCollision(Player *player) {
    SDL_Rect playerRect = { 
        (int)(player->position.x * escala), 
        (int)(player->position.y * escala), 
        (int)(player->width * escala), 
        (int)(player->height * escala) 
    };

    player->position.onGround = false;  // Assume que o jogador está no ar até encontrar um bloco embaixo

    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            if (gameMap.tiles[y][x] > 0) {  // Somente checar blocos sólidos
                SDL_Rect blockRect = { 
                    (int)(x * 64 * escala), 
                    (int)(y * 64 * escala), 
                    (int)(64 * escala), 
                    (int)(64 * escala) 
                };

                if (checkCollision(playerRect, blockRect)) {
                    // Colisão por cima (o jogador aterrissa no bloco)
                    if (playerRect.y + playerRect.h <= blockRect.y + 5 && player->position.velY >= 0) {
                        player->position.onGround = true;
                        player->position.y = blockRect.y / escala - player->height;  // Define a posição exata em cima do bloco
                        player->position.velY = 0;  // Zera a velocidade vertical
                        return true;
                    }
                    // Colisão por baixo (o jogador bate a cabeça no bloco)
                    else if (playerRect.y >= blockRect.y + blockRect.h - 5 && player->position.velY < 0) {
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

// Função para verificar colisões entre o jogador e blocos
bool checkEnemyBlockCollision(Enemy *enemy) {
    SDL_Rect enemyRect = { 
        (int)(enemy->position.x * escala), 
        (int)(enemy->position.y * escala), 
        (int)(enemy->width * escala), 
        (int)(enemy->height * escala) 
    };

    enemy->position.onGround = false;  // Assume que o jogador está no ar até encontrar um bloco embaixo

    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            if (gameMap.tiles[y][x] > 0) {  // Somente checar blocos sólidos
                SDL_Rect blockRect = { 
                    (int)(x * 64 * escala), 
                    (int)(y * 64 * escala), 
                    (int)(64 * escala), 
                    (int)(64 * escala) 
                };

                if (checkCollision(enemyRect, blockRect)) {
                    // Colisão por cima (o jogador aterrissa no bloco)
                    if (enemyRect.y + enemyRect.h <= blockRect.y + 5 && enemy->position.velY >= 0) {
                        enemy->position.onGround = true;
                        enemy->position.y = blockRect.y / escala - enemy->height;  // Define a posição exata em cima do bloco
                        enemy->position.velY = 0;  // Zera a velocidade vertical
                        return true;
                    }
                    // Colisão por baixo (o jogador bate a cabeça no bloco)
                    else if (enemyRect.y >= blockRect.y + blockRect.h - 5 && enemy->position.velY < 0) {
                        enemy->position.y = (blockRect.y + blockRect.h) / escala;
                        enemy->position.velY = 0;  // Zera a velocidade vertical ao colidir por baixo
                        return true;
                    }
                    // Colisão lateral (ajustes de posição horizontal)
                    else if (enemyRect.x + enemyRect.w > blockRect.x && enemyRect.x < blockRect.x) {
                        enemy->position.x = blockRect.x / escala - enemy->width;  // Ajuste suave pela esquerda
                        enemy->position.velX = 0;  // Zera a velocidade horizontal
                    } 
                    else if (enemyRect.x < blockRect.x + blockRect.w && enemyRect.x > blockRect.x) {
                        enemy->position.x = (blockRect.x + blockRect.w) / escala;  // Ajuste suave pela direita
                        enemy->position.velX = 0;  // Zera a velocidade horizontal
                    }
                }
            }
        }
    }
    return false;
}
