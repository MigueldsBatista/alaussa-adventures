#ifndef __ENEMY_H__
#define __ENEMY_H__
#define MAX_ENEMIES 5


#include <stdbool.h>
#include <SDL2/SDL.h>
/*
Funções para o comportamento e animação dos inimigos.
*/

/**
 * @struct Enemy
 * @brief Represents an enemy entity in the game.
 * 
 * This structure holds the properties of an enemy, including its position,
 * speed, health, activity status, and texture for rendering.
 * 
 * @param x
 * The x-coordinate of the enemy's position.
 * 
 * @param y
 * The y-coordinate of the enemy's position.
 * 
 * @param speedX
 * The speed of the enemy along the x-axis.
 * 
 * @param speedY
 * The speed of the enemy along the y-axis.
 * 
 * @param health
 * The health of the enemy.
 * 
 * @param isActive
 * The activity status of the enemy.
 * 
 * @param texture
 * The SDL texture used for rendering the enemy.
 */
typedef struct {

    int x, y;

    int speedX, speedY;

    int health;

    bool isActive;

    SDL_Texture **animationFrames;

    int totalFrames;

    int currentFrame;

    double animationTime;

} Enemy;

typedef enum {
     ENEMY_IDLE, //repouso
     ENEMY_MOVE_LEFT,
     ENEMY_MOVE_RIGHT,
} EnemyAction;

typedef struct {
    Enemy enemies[MAX_ENEMIES];
    int currentEnemyIndex;
    double spawnTimer;
} EnemyQueue;

/**
 * @brief Initializes the enemy entity.
 *
 * This function sets up the initial state of the enemy, including its
 * position, health, and any other relevant attributes. It should be called
 * before the enemy is used in the game.
 */
void initEnemy(Enemy *enemy, int x, int y, int speedX, int speedY, int health, SDL_Texture **animationFrames, int totalFrames, SDL_Renderer *renderer);

/**
 * @brief Updates the state of the enemy.
 *
 * This function is responsible for updating the enemy's state, which may include
 * position, health, behavior, and other attributes relevant to the game's logic.
 */
void updateEnemy(Enemy *enemy, double deltaTime);

/**
 * @brief Renders the enemy on the screen.
 *
 * This function is responsible for drawing the enemy character
 * on the screen. It handles all the necessary graphical operations
 * to ensure the enemy is displayed correctly.
 */
void renderEnemy(Enemy *enemy, SDL_Renderer *renderer);

void initEnemyStack(Enemy *enemies, int numEnemies, SDL_Texture **animationFrames, int totalFrames, SDL_Renderer *renderer);

void loadAnimationsEnemy(Enemy *enemy, SDL_Renderer *renderer);

void deactivateEnemy(Enemy *enemy);

void damageEnemy(Enemy *enemy, int damage);

void freeEnemyAnimations(Enemy *enemy);

void initEnemyQueue(EnemyQueue *queue);

void spawnEnemy(EnemyQueue *queue, int x, int y, int speedX, int speedY, int health, SDL_Texture **animationFrames, int totalFrames, SDL_Renderer *renderer);

void updateEnemyQueue(EnemyQueue *queue, double deltaTime);

void renderEnemyQueue(EnemyQueue *queue, SDL_Renderer *renderer);

#endif // __ENEMY_H__


