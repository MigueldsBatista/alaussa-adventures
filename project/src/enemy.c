#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "enemy.h"
#include "map.h"
#include "player.h"
#include <stdbool.h>

const float escala2 = 0.5;

// Estrutura de dados para o mapa
typedef struct {
    int width;
    int height;
    int tiles[MAX_MAP_HEIGHT][MAX_MAP_WIDTH]; // Exemplo de representação do mapa
} Map;

Map gameEnemyMap; // Instância do mapa

void spawnEnemiesFromMap(const char* map_file, DoubleLinkedListEnemy **enemyList) {
    FILE* file = fopen(map_file, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo do mapa: %s\n", map_file);
        return;
    }

    // Lê a largura e a altura do mapa
    if (fscanf(file, "%d %d", &gameEnemyMap.width, &gameEnemyMap.height) != 2) {
        fprintf(stderr, "Erro ao ler largura e altura do mapa.\n");
        fclose(file);
        return;
    }

    // Leitura dos tiles do mapa
    for (int y = 0; y < gameEnemyMap.height; y++) {
        for (int x = 0; x < gameEnemyMap.width; x++) {
            // Lê o valor do tile diretamente na estrutura do mapa
            if (fscanf(file, "%d", &gameEnemyMap.tiles[y][x]) != 1) {
                fprintf(stderr, "Erro ao ler tile no mapa na posição (%d, %d).\n", x, y);
                fclose(file);
                return;
            }

            // Verifica se o tile corresponde a um inimigo
            if (gameEnemyMap.tiles[y][x] == 2) {
                Enemy *newEnemy = (Enemy *)malloc(sizeof(Enemy));
                if (!newEnemy) {
                    fprintf(stderr, "Erro ao alocar memória para o novo inimigo.\n");
                    fclose(file);
                    return;
                }
                
                newEnemy->position.x = x * 32; 
                newEnemy->position.y = y * 32; 
                newEnemy->position.velX = 0;    
                newEnemy->position.velY = 0;    
                newEnemy->position.onGround = true; 
                newEnemy->width = 32;  
                newEnemy->height = 32; 
                newEnemy->totalFrames = 0; 
                newEnemy->currentFrame = 0; 
                newEnemy->currentEnemyAction = ENEMY_MOVE_LEFT;
                initLifeEnemy(newEnemy);
                
                // Incrementar cont para IDs únicos
                static int cont = 0; // Usar cont como variável estática para manter o estado
                cont++;
                
                addEnemyDoubleLinkedList(enemyList, newEnemy, cont);
            }
        }
    }
    fclose(file);
}


void initLifeEnemy(Enemy *enemy) {
    enemy->Becker = NULL;
    LifeEnemy *vida; // Move declaration here to avoid reinitializing inside the loop
    for (int i = 0; i < 1; i++) {
        vida = (LifeEnemy *)malloc(sizeof(LifeEnemy));
        vida->id = i + 1;               
        vida->prox = enemy->Becker;    
        enemy->Becker = vida;          
    }
}

void addEnemyDoubleLinkedList(DoubleLinkedListEnemy **head, Enemy *newEnemy, int enemyId) {
    DoubleLinkedListEnemy *newNode = (DoubleLinkedListEnemy *)malloc(sizeof(DoubleLinkedListEnemy));
    if (newNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o novo inimigo.\n");
        return;
    }

    newNode->id = enemyId;
    newNode->enemy = newEnemy;
    newNode->prox = NULL;

    if (*head == NULL) {
        newNode->ant = NULL;  
        *head = newNode;      
        return;
    }

    DoubleLinkedListEnemy *temp = *head;
    while (temp->prox != NULL) {
        temp = temp->prox;
    }

    temp->prox = newNode;
    newNode->ant = temp;
    return;
}

void updateEnemy(Enemy *enemy, double gravity, double deltaTime, Player *player, SDL_Renderer *renderer) {
    enemy->position.velY += gravity * deltaTime;

    if (enemy->position.velY > 100) {
        enemy->position.velY = 100;
    }

    if (enemy->position.velX > 150) {
        enemy->position.velX = 150;
    }

    if (enemy->position.velX < -150) {
        enemy->position.velX = -150;
    }

    enemy->position.x += (enemy->position.velX * deltaTime);
    enemy->position.y += (enemy->position.velY * deltaTime);

    if (checkPlayerEnemyColision(player, enemy)) {
        damageEnemy(enemy);
        killEnemyIfDead(enemy);
    }

    if (checkEnemyBlockCollision(enemy)) {
        enemy->position.velY = 0; 
        enemy->position.onGround = true;
    }

    if (enemy->position.velX > 0) {
        enemy->currentFrame = (enemy->currentFrame + 1) % enemy->totalFrames;
        freeEnemyAnimationFrames(enemy);
        loadEnemyAnimationFrames(enemy, ENEMY_MOVE_RIGHT, renderer);
    } else {
        enemy->currentFrame = (enemy->currentFrame + 1) % enemy->totalFrames;
        freeEnemyAnimationFrames(enemy);
        loadEnemyAnimationFrames(enemy, ENEMY_MOVE_LEFT, renderer);
    }
}

bool loadEnemyAnimationFrames(Enemy *enemy, EnemyAction action, SDL_Renderer *renderer) {
    int frameCount = 0;
    switch (action) {
        case ENEMY_MOVE_LEFT:
        case ENEMY_MOVE_RIGHT: frameCount = 2; break;
        default: break;
    }

    enemy->animationEnemyFrames = malloc(sizeof(SDL_Texture*) * frameCount);
    enemy->totalFrames = frameCount;

    for (int i = 0; i < frameCount; i++) {
        char filename[100];
        if (action == ENEMY_MOVE_LEFT) {
            sprintf(filename, "project/assets/MovEnemy/enemy_left_%d.png", i);
        } else if (action == ENEMY_MOVE_RIGHT) {
            sprintf(filename, "project/assets/MovEnemy/enemy_right_%d.png", i);
        }
        
        SDL_Surface *surface = IMG_Load(filename);
        if (surface == NULL) {
            printf("Falha ao carregar a imagem: %s\n", IMG_GetError());
            enemy->animationEnemyFrames[i] = NULL;
        } else {
            enemy->animationEnemyFrames[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
    return true; // Added return statement
}

void freeEnemyAnimationFrames(Enemy *enemy) {
    if (enemy->animationEnemyFrames != NULL) { // Check if animationFrames is not NULL
        for (int i = 0; i < enemy->totalFrames; i++) {
            if (enemy->animationEnemyFrames[i] != NULL) {
                SDL_DestroyTexture(enemy->animationEnemyFrames[i]);
            }
        }
        free(enemy->animationEnemyFrames);
        enemy->animationEnemyFrames = NULL;
    }
}

void damageEnemy(Enemy *enemy) {
    if (enemy->Becker != NULL) { // Check if Becker is not NULL
        LifeEnemy *temp = enemy->Becker;
        enemy->Becker = enemy->Becker->prox;
        free(temp);
    }
}

bool checkEnemyCollision(SDL_Rect a, SDL_Rect b) {
    return !(a.x + a.w <= b.x || a.x >= b.x + b.w || a.y + a.h <= b.y || a.y >= b.y + b.h);
}

bool checkPlayerEnemyColision(Player *player, Enemy *enemy) {
    SDL_Rect playerRect = { 
        (int)(player->position.x * escala2), 
        (int)(player->position.y * escala2), 
        (int)(player->width * escala2), 
        (int)(player->height * escala2) 
    };

    SDL_Rect enemyRect = { 
        (int)(enemy->position.x * escala2), 
        (int)(enemy->position.y * escala2), 
        (int)(enemy->width * escala2), 
        (int)(enemy->height * escala2) 
    };

    if (checkEnemyCollision(playerRect, enemyRect)) {
        if (playerRect.y + playerRect.h <= enemyRect.y + 5 && player->position.velY >= 0) {
            damageEnemy(enemy);
            return false;
        } else if (playerRect.x + playerRect.w > enemyRect.x && playerRect.x < enemyRect.x) {
            damagePlayer(player);
            player->position.x -= 7;
            return true;
        } else if (playerRect.x < enemyRect.x + enemyRect.w && playerRect.x > enemyRect.x) {
            damagePlayer(player);
            player->position.x += 7;
            return true;
        }
    }
    return false;
}

bool enemyHasLife(Enemy *enemy) {
    return enemy->Becker != NULL;
}

void killEnemyIfDead(Enemy *enemy) {
    if (!enemyHasLife(enemy)) {
        printf("O inimigo foi derrotado!\n");
        LifeEnemy *currentLife = enemy->Becker;
        while (currentLife != NULL) {
            LifeEnemy *temp = currentLife;
            currentLife = currentLife->prox;
            free(temp);
        }
        freeEnemyAnimationFrames(enemy);
        enemy->Becker = NULL;
    }
    return;
}

// Função para verificar colisões entre o jogador e blocos
bool checkEnemyBlockCollision(Enemy *enemy) {
    SDL_Rect enemyRect = { 
        (int)(enemy->position.x * escala2), 
        (int)(enemy->position.y * escala2), 
        (int)(enemy->width * escala2), 
        (int)(enemy->height * escala2) 
    };

    enemy->position.onGround = false;  // Assume que o jogador está no ar até encontrar um bloco embaixo

    for (int y = 0; y < gameEnemyMap.height; y++) {
        for (int x = 0; x < gameEnemyMap.width; x++) {
            if (gameEnemyMap.tiles[y][x] > 0) {  // Somente checar blocos sólidos
                SDL_Rect blockRect = { 
                    (int)(x * 64 * escala2), 
                    (int)(y * 64 * escala2), 
                    (int)(64 * escala2), 
                    (int)(64 * escala2) 
                };

                if (checkCollision(enemyRect, blockRect)) {
                    // Colisão por cima (o jogador aterrissa no bloco)
                    if (enemyRect.y + enemyRect.h <= blockRect.y + 5 && enemy->position.velY >= 0) {
                        enemy->position.onGround = true;
                        enemy->position.y = blockRect.y / escala2 - enemy->height;  // Define a posição exata em cima do bloco
                        enemy->position.velY = 0;  // Zera a velocidade vertical
                        return true;
                    }
                    // Colisão por baixo (o jogador bate a cabeça no bloco)
                    else if (enemyRect.y >= blockRect.y + blockRect.h - 5 && enemy->position.velY < 0) {
                        enemy->position.y = (blockRect.y + blockRect.h) / escala2;
                        enemy->position.velY = 0;  // Zera a velocidade vertical ao colidir por baixo
                        return true;
                    }
                    // Colisão lateral (ajustes de posição horizontal)
                    else if (enemyRect.x + enemyRect.w > blockRect.x && enemyRect.x < blockRect.x) {
                        enemy->position.x = blockRect.x / escala2 - enemy->width;  // Ajuste suave pela esquerda
                        enemy->position.velX = 0;  // Zera a velocidade horizontal
                    } 
                    else if (enemyRect.x < blockRect.x + blockRect.w && enemyRect.x > blockRect.x) {
                        enemy->position.x = (blockRect.x + blockRect.w) / escala2;  // Ajuste suave pela direita
                        enemy->position.velX = 0;  // Zera a velocidade horizontal
                    }
                }
            }
        }
    }
    return false;
}


void renderEnemy(Enemy *enemy, SDL_Renderer *renderer) {
    SDL_Rect dstRect = {
        .x = (int)enemy->position.x,
        .y = (int)enemy->position.y,
        .w = enemy->width,
        .h = enemy->height
    };
    SDL_RenderCopy(renderer, enemy->animationEnemyFrames[enemy->currentFrame], NULL, &dstRect);
}