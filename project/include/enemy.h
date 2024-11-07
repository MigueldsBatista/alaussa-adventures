#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "map.h"
#include "player.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

// Estrutura para a posição do inimigo
typedef struct PositionEnemy {
    double x, y;       
    double velX, velY; 
    bool onGround;     
} PositionEnemy;

// Estrutura para representar a vida do inimigo
typedef struct LifeEnemy {
    int id;
    struct LifeEnemy *prox;
} LifeEnemy;

// Ações que o inimigo pode executar
typedef enum {
    ENEMY_MOVE_LEFT,
    ENEMY_MOVE_RIGHT,
} EnemyAction;

// Estrutura que representa um inimigo
typedef struct Enemy {
    PositionEnemy position;
    int width;
    int height;
    LifeEnemy *Becker; 
    SDL_Texture **animationEnemyFrames;
    int totalFrames;
    int currentFrame;
    EnemyAction currentEnemyAction;
} Enemy;

// Estrutura para uma lista duplamente encadeada de inimigos
typedef struct DoubleLinkedListEnemy {
    int id;
    Enemy *enemy;
    struct DoubleLinkedListEnemy *prox;
    struct DoubleLinkedListEnemy *ant;
} DoubleLinkedListEnemy;

// Declarações de funções
void spawnEnemiesFromMap(const char* map_file, DoubleLinkedListEnemy **enemyList);

void initLifeEnemy(Enemy *enemy);

void addEnemyDoubleLinkedList(DoubleLinkedListEnemy **head, Enemy *newEnemy, int enemyId);

void updateEnemy(Enemy *enemy, double gravity, double deltaTime, Player *player, SDL_Renderer *renderer);

bool loadEnemyAnimationFrames(Enemy *enemy, EnemyAction action, SDL_Renderer *renderer);

void freeEnemyAnimationFrames(Enemy *enemy);

bool checkEnemyCollision(SDL_Rect a, SDL_Rect b);

bool checkPlayerEnemyColision(Player *player, Enemy *enemy);

bool enemyHasLife(Enemy *enemy);

void damageEnemy(Enemy *enemy);

void killEnemyIfDead(Enemy *enemy);


#endif // __ENEMY_H__
