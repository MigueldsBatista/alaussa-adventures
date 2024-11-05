#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "enemy.h"

#define SCREEN_WIDTH 725
#define SCREEN_HEIGHT 400

void initEnemyStack(Enemy *enemies, int numEnemies, SDL_Texture **animationFrames, int totalFrames, SDL_Renderer *renderer) {
    for (int i = 0; i < numEnemies; i++) {
        int x = rand() % (SCREEN_WIDTH - 64);
        int y = rand() % (SCREEN_HEIGHT - 64);
        int speedX = (rand() % 200) - 100; // Velocidade aleatória entre -100 e 100
        int speedY = (rand() % 200) - 100; // Velocidade aleatória entre -100 e 100
        int health = 150; // Saúde = 150
        initEnemy(&enemies[i], x, y, speedX, speedY, health, animationFrames, totalFrames, renderer);
    }
}

void renderEnemy(Enemy *enemy, SDL_Renderer *renderer) {
    if (enemy->isActive && enemy->animationFrames[enemy->currentFrame] != NULL) {
        SDL_Rect dstRect;
        dstRect.x = enemy->x;
        dstRect.y = enemy->y;
        dstRect.w = 64;
        dstRect.h = 64;

        SDL_RenderCopy(renderer, enemy->animationFrames[enemy->currentFrame], NULL, &dstRect);
    }
}

void initEnemy(Enemy *enemy, int x, int y, int speedX, int speedY, int health, SDL_Texture **animationFrames, int totalFrames, SDL_Renderer *renderer) {
    enemy->x = x;
    enemy->y = y;
    enemy->speedX = speedX;
    enemy->speedY = speedY;
    enemy->health = health;
    enemy->isActive = true;
    enemy->animationFrames = animationFrames;
    enemy->totalFrames = totalFrames;
    enemy->currentFrame = 0;
    enemy->animationTime = 0.0;

    loadAnimationsEnemy(enemy, renderer);
}

void loadAnimationsEnemy(Enemy *enemy, SDL_Renderer *renderer) {
    if (enemy->animationFrames != NULL) {
        for (int i = 0; i < enemy->totalFrames; i++) {
            if (enemy->animationFrames[i] != NULL) {
                SDL_DestroyTexture(enemy->animationFrames[i]);
            }
        }
        free(enemy->animationFrames);
    }

    int frameCount = 2; // Número de frames para a animação de idle
    enemy->animationFrames = malloc(sizeof(SDL_Texture*) * frameCount);
    enemy->totalFrames = frameCount;

    for (int i = 0; i < frameCount; i++) {
        char filename[50];
        snprintf(filename, sizeof(filename), "./project/assets/MovEnemy/enemy_idle_%d.png", i);

        SDL_Surface *surface = IMG_Load(filename);
        if (surface == NULL) {
            printf("Failed to load image: %s\n", filename);
            enemy->animationFrames[i] = NULL; // Marque como NULL para indicar erro
        } else {
            enemy->animationFrames[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (enemy->animationFrames[i] == NULL) {
                printf("Failed to create texture from surface for %s\n", filename);
            }
        }
    }
}

void updateEnemy(Enemy *enemy, double deltaTime) {
    if (enemy->isActive) {
        // Atualiza a posição do inimigo com base na sua velocidade e deltaTime
        enemy->x += enemy->speedX * deltaTime;
        enemy->y += enemy->speedY * deltaTime;

        // Lógica para manter o inimigo dentro dos limites da tela
        if (enemy->x < 0) {
            enemy->x = 0;
            enemy->speedX = -enemy->speedX; // Inverte a velocidade ao colidir com a borda esquerda
        } else if (enemy->x + 64 > SCREEN_WIDTH) {
            enemy->x = SCREEN_WIDTH - 64;
            enemy->speedX = -enemy->speedX; // Inverte a velocidade ao colidir com a borda direita
        }

        if (enemy->y < 0) {
            enemy->y = 0;
            enemy->speedY = -enemy->speedY; // Inverte a velocidade ao colidir com a borda superior
        } else if (enemy->y + 64 > SCREEN_HEIGHT) {
            enemy->y = SCREEN_HEIGHT - 64;
            enemy->speedY = -enemy->speedY; // Inverte a velocidade ao colidir com a borda inferior
        }

        // Atualiza a animação do inimigo
        enemy->animationTime += deltaTime;
        if (enemy->animationTime >= 0.1) { // Alterna a cada 0.1 segundos
            enemy->currentFrame = (enemy->currentFrame + 1) % enemy->totalFrames;
            enemy->animationTime = 0.0;
        }
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

void freeEnemyAnimations(Enemy *enemy) {
    if (enemy->animationFrames != NULL) {
        for (int i = 0; i < enemy->totalFrames; i++) {
            if (enemy->animationFrames[i] != NULL) {
                SDL_DestroyTexture(enemy->animationFrames[i]);
            }
        }
        free(enemy->animationFrames);
        enemy->animationFrames = NULL;
    }
}
