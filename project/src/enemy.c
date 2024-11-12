#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include <SDL2/SDL_ttf.h>
EnemyList enemyList = { .enemyCount = 0 };

void moveEnemyLeft(Entity *entity) {
    entity->position.velX = -12.5;
}

void moveEnemyRight(Entity *entity) {
    entity->position.velX = 12.5;
}

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
        enemy->position.velY += GRAVIDADE * DELTA_TIME;
        if (enemy->position.velY > 100) {
            enemy->position.velY = 100;
        }
        if (enemy->position.y + enemy->height >= GROUND_LEVEL) {
            enemy->position.y = GROUND_LEVEL - enemy->height;
            enemy->position.velY = 0;
            enemy->position.onGround = true;
        }
        // ↑ esse codigo de cima é pra caso o inimigo esteja fora do chão aplicar a gravidade e essas coisas
        // Movimento
if (enemy->position.onGround) {
    // Se o inimigo estiver parado, começa movendo para a esquerda
    if (enemy->position.velX == 0) {
        moveEnemyLeft(enemy);
    }

    // Verifica colisão com blocos e inverte a direção
    if (checkEntityBlockCollision(enemy)) {
        // Inverte a direção horizontal ao colidir
        enemy->position.velX = -enemy->position.velX;

        // Atualiza a direção com base na nova velocidade
        if (enemy->position.velX > 0) {
            moveEnemyRight(enemy);
        } else if(enemy->position.velX < 0) {
            moveEnemyLeft(enemy);
        }
    }

    // Verifica a borda da plataforma e inverte a direção, se necessário
    checkPlatformEdge(enemy);
}

        // Atualiza a posição horizontal do inimigo
        enemy->position.x += enemy->position.velX * DELTA_TIME;
        // Atualiza (queda)
        enemy->position.y += enemy->position.velY * DELTA_TIME;
        // Renderiza o inimigo
        updateEntity(enemy, renderer);
    }
}


void freeEnemyList() {
    for (int i = 0; i < enemyList.enemyCount; i++) {
        free(enemyList.enemies[i]);
        printf("Inimigo %d liberado\n", i);
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
        SDL_Rect enemyRect = {(int)enemy->position.x,(int)enemy->position.y,enemy->width,enemy->height};
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            if (player->imortalidadeAtiva) {
                return;
            }
            // Verifica se o jogador caiu em cima do inimigo
            if (playerRect.y + playerRect.h >= enemyRect.y && playerRect.y + playerRect.h < enemyRect.y + enemyRect.h / 2 && player->position.velY >= 0) {
                player->moedas=player->moedas+1;
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