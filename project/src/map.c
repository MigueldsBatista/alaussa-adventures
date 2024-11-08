#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "map.h"
#include "entity.h"
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

// The first line contains the width and height of the map.
// The following lines contain the tile values for each row of the map.
void renderMap(SDL_Renderer* renderer, SDL_Texture* bloco_texture) {
    //TODO: Implementar a função renderMap
    for (int y = 0; y < gameMap.height; y++) {
        for (int x = 0; x < gameMap.width; x++) {
            int tile = gameMap.tiles[y][x];
            if (tile == 1) { // Supondo que um tile "0" é vazio
                SDL_Rect dst_rect = { x * 64, y * 64, 64, 64 }; // Define a posição e o tamanho do tile
                SDL_RenderCopy(renderer, bloco_texture, NULL, &dst_rect);
                //printf("Bloco renderizado em (%d, %d)\n", x, y);
            }
            if(tile == 2){
                addEnemy(x * 64, y * 64, renderer);
                gameMap.tiles[y][x] = 0; // Marca o tile como vazio para evitar adição duplicada        }
            }
            if(tile!=1 && tile!=2){
                gameMap.tiles[y][x]=0;//esse if é so pra caso a gente bote algum numero que a gente não quer ele só ignora e bota bloco de ar
            }
        }
    }
}

void renderBackground(SDL_Renderer* renderer, SDL_Texture* background, int camera_position, int screen_width, int screen_height) {
    //TODO: Implementar a função renderBackground
    int bg_width = 725;  // Largura da imagem de fundo
    int bg_height = 400; // Altura da imagem de fundo

    // Calcula o deslocamento do parallax (horizontal)
    int parallax_offset = camera_position % bg_width;

    // Renderiza a primeira parte da imagem
    SDL_Rect src_rect1 = { parallax_offset, 0, bg_width - parallax_offset, bg_height };
    SDL_Rect dst_rect1 = { 0, 0, bg_width - parallax_offset, bg_height };
    SDL_RenderCopy(renderer, background, &src_rect1, &dst_rect1);

    // Se houver espaço sobrando, renderiza a segunda parte da imagem para "repeti-la"
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
