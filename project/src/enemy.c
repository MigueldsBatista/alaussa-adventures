#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "enemy.h"

#define SCREEN_WIDTH 725
#define SCREEN_HEIGHT 400


void initEnemy(Enemy *enemy, int x, int y, int speedX, int speedY, int health, SDL_Texture *texture) {
    enemy->x = x;
    enemy->y = y;
    enemy->speedX = speedX;
    enemy->speedY = speedY;
    enemy->health = health;
    enemy->isActive = true;
    enemy->texture = texture;
}

void updateEnemy(Enemy *enemy, double deltaTime) {
    if (enemy->isActive) {
        // Atualiza a posição do inimigo com base na sua velocidade e deltaTime
        enemy->x += enemy->speedX * deltaTime;
        enemy->y += enemy->speedY * deltaTime;

        // Lógica para manter o inimigo dentro dos limites da tela
        if (enemy->x < 0) {
            enemy->x = 0;
            enemy->speedX = -enemy->speedX; // Inverte a direção ao colidir com a borda esquerda
        } else if (enemy->x + 64 > SCREEN_WIDTH) {
            enemy->x = SCREEN_WIDTH - 64;
            enemy->speedX = -enemy->speedX; // Inverte a direção ao colidir com a borda direita
        }

        if (enemy->y < 0) {
            enemy->y = 0;
            enemy->speedY = -enemy->speedY; // Inverte a direção ao colidir com a borda superior
        } else if (enemy->y + 64 > SCREEN_HEIGHT) {
            enemy->y = SCREEN_HEIGHT - 64;
            enemy->speedY = -enemy->speedY; // Inverte a direção ao colidir com a borda inferior
        }
    }
}

void renderEnemy(Enemy *enemy, SDL_Renderer *renderer) {
    if (enemy->isActive) {
        SDL_Rect dstRect;
        dstRect.x = enemy->x;
        dstRect.y = enemy->y;
        dstRect.w = 64; // Largura do inimigo
        dstRect.h = 64; // Altura do inimigo

        SDL_RenderCopy(renderer, enemy->texture, NULL, &dstRect);
    }
}
void deactivateEnemy(Enemy *enemy) {
    enemy->isActive = false;
}

void damageEnemy(Enemy *enemy, int damage) {
    if (enemy->isActive) {
        enemy->health -= damage;
        if (enemy->health <= 0) {
            deactivateEnemy(enemy);
        }
    }
}