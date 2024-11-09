// enemy.c

#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>
#include "map.h"
EnemyList enemyList = { .enemyCount = 0 };

void addEnemy(int x, int y, SDL_Renderer *renderer) {
    if (enemyList.enemyCount < MAX_ENEMIES) {
        Entity *enemy = (Entity *)malloc(sizeof(Entity));
        if (enemy) {
            initEnemy(enemy, ENEMY, x, y, 3, renderer);
            enemyList.enemies[enemyList.enemyCount++] = enemy;
            printf("Inimigo adicionado em (%d, %d)\n", x, y);
        } else {
            fprintf(stderr, "Falha ao alocar memória para inimigo.\n");
        }
    } else {
        fprintf(stderr, "Limite máximo de inimigos atingido.\n");
    }
}

void renderEnemies(SDL_Renderer *renderer) {
    for (int i = 0; i < enemyList.enemyCount; i++) {
        Entity *enemy = enemyList.enemies[i];
        renderEntity(enemy, renderer);
    }
}

void updateEnemies(SDL_Renderer *renderer) {
    for (int i = 0; i < enemyList.enemyCount; i++) {
        Entity *enemy = enemyList.enemies[i];
        if(enemy->position.y + enemy->height >= GROUND_LEVEL){
            enemy->position.y = GROUND_LEVEL - enemy->height;
            enemy->position.velY = 0;
            enemy->position.onGround = true;
            moveRight(enemy);
        }
        else if (enemy->position.x + enemy->width > SCREEN_WIDTH) {
            enemy->position.x = SCREEN_WIDTH - enemy->width;    
            moveLeft(enemy);    
        }  
        updateEntity(enemy, renderer);
    
    }
}


void freeEnemyList() {
    for (int i = 0; i < enemyList.enemyCount; i++) {
        free(enemyList.enemies[i]);
    }
    enemyList.enemyCount = 0;
}


void checkPlayerEnemyCollision(Entity *player, EnemyList *enemyList) {
    SDL_Rect playerRect = {
        (int)player->position.x,
        (int)player->position.y,
        player->width,
        player->height
    };
    
    for (int i = 0; i < enemyList->enemyCount; i++) {
        Entity *enemy = enemyList->enemies[i];
        SDL_Rect enemyRect = {
            (int)enemy->position.x,
            (int)enemy->position.y,
            enemy->width,
            enemy->height
        };
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            if (player->imortalidadeAtiva) {
                return;
            }
            // Verifica se o jogador caiu em cima do inimigo
            if (playerRect.y + playerRect.h >= enemyRect.y && playerRect.y + playerRect.h < enemyRect.y + enemyRect.h / 2 && player->position.velY >= 0) {
                free(enemyList->enemies[i]);  // Libera a memória do inimigo atual
                // Remove o inimigo da lista, reorganizando o array
                for (int j = i; j < enemyList->enemyCount - 1; j++) {
                    enemyList->enemies[j] = enemyList->enemies[j + 1];
                }

                enemyList->enemyCount--;
                enemyList->enemies[enemyList->enemyCount] = NULL;  // Garante que o último ponteiro seja nulo
                player->position.velY = -150;  // o pulinho de hurray!!
            }
            // Colisão lateral ou por baixo
            
            else if(playerRect.x + playerRect.w > enemyRect.x && playerRect.x < enemyRect.x + enemyRect.w){
                damageEntity(player);
            }
        }
        
    }
}
