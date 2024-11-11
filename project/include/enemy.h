#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "entity.h"
#include <SDL2/SDL.h>
#include <stdlib.h> 
#include "entity.h"
#include "game.h"
#define MAX_ENEMIES 10

typedef struct {
    Entity *enemies[MAX_ENEMIES];
    int enemyCount;
} EnemyList;

extern EnemyList enemyList;

void initEnemyList(EnemyList *list);
void addEnemy(int x, int y, SDL_Renderer *renderer);
void updateEnemies(SDL_Renderer *renderer);
void renderEnemies(SDL_Renderer *renderer);
void freeEnemyList();
void checkPlayerEnemyCollision(Entity *player, EnemyList *enemyList);

void moveEnemyLeft(Entity *entity);
void moveEnemyRight(Entity *entity);

bool checkEntityBlockCollision(Entity *enemy);



#endif // !__ENEMY_H__