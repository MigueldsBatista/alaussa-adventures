#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <SDL2/SDL.h>
/*
Funções para o comportamento e animação dos inimigos.
*/

/**
 * @struct Enemy
 * @brief Represents an enemy entity in the game.
 * 
 * This structure holds the properties of an enemy, including its source and 
 * destination rectangles for rendering, as well as its position and velocity.
 * 
 * @param sourceRectangle
 * Source rectangle for rendering the enemy sprite.
 * 
 * @param destinationRectangle
 * Destination rectangle for rendering the enemy sprite.
 * 
 * @param x
 * The x-coordinate of the enemy's position.
 * 
 * @param y
 * The y-coordinate of the enemy's position.
 * 
 * @param velocityX
 * The velocity of the enemy along the x-axis.
 */
typedef struct {
     SDL_Rect sourceRectangle, destinationRectangle; 
     int x, y, velocityX;
} Enemy;

/**
 * @brief Initializes the enemy entity.
 *
 * This function sets up the initial state of the enemy, including its
 * position, health, and any other relevant attributes. It should be called
 * before the enemy is used in the game.
 */
void initEnemy();

/**
 * @brief Updates the state of the enemy.
 *
 * This function is responsible for updating the enemy's state, which may include
 * position, health, behavior, and other attributes relevant to the game's logic.
 */
void updateEnemy();

/**
 * @brief Renders the enemy on the screen.
 *
 * This function is responsible for drawing the enemy character
 * on the screen. It handles all the necessary graphical operations
 * to ensure the enemy is displayed correctly.
 */
void renderEnemy();


#endif // __ENEMY_H__
