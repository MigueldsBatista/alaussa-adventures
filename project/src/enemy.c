#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "enemy.h"
#include "player.h"
#include "map.h"
#include <stdbool.h>

const float escala = 0.5;

void spawnEnemiesFromMap(const char* map_file, DoubleLinkedListEnemy **enemyList) {
    FILE* file = fopen(map_file, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo do mapa: %s\n", map_file);
        return;
    }

    int width, height;
    fscanf(file, "%d %d", &width, &height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tile;
            fscanf(file, "%d", &tile);
            if (tile == 2) {
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
                int cont = 0;
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

    if (enemy->position.velY > 120) {
        enemy->position.velY = 120;
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
        (int)(player->position.x * escala), 
        (int)(player->position.y * escala), 
        (int)(player->width * escala), 
        (int)(player->height * escala) 
    };

    SDL_Rect enemyRect = { 
        (int)(enemy->position.x * escala), 
        (int)(enemy->position.y * escala), 
        (int)(enemy->width * escala), 
        (int)(enemy->height * escala) 
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
