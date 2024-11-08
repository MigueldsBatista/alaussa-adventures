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
            initEntity(enemy, ENEMY, x, y, 3, renderer);
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
