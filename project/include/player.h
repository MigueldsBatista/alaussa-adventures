#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <SDL2/SDL.h>

/**
 * @brief Structure that represents the position of an object or character in the game.
 * 
 * The Position structure stores the x and y coordinates, where:
 * - x: Horizontal coordinate.
 * - y: Vertical coordinate.
 */
typedef struct {
    int x; ///< Horizontal coordinate (x position).
    int y; ///< Vertical coordinate (y position).
} Position;

/**
 * @brief Initializes the player.
 * 
 * This function sets up the initial state of the player, including
 * any necessary variables, resources, or configurations required
 * for the player to function correctly in the game.
 */
void initPlayer();

/**
 * @brief Updates the player's state.
 *
 * This function is responsible for updating the player's state, including
 * position, health, and other relevant attributes. It should be called
 * regularly to ensure the player's state is kept up-to-date.
 */
void updatePlayer();

/**
 * @brief Renders the player on the screen.
 *
 * This function is responsible for drawing the player character
 * on the screen. It handles all the necessary graphical operations
 * to ensure the player is displayed correctly.
 */
void renderPlayer();

/**
 * @brief Handles the player's input events.
 * 
 * This function processes the SDL_Event passed to it and updates the player's
 * state accordingly based on the input received.
 * 
 * @param event Pointer to an SDL_Event structure that contains the event data.
 */
void handlePlayerInput(SDL_Event *event);

/**
 * @brief Moves the object up, decreasing the y value.
 * 
 * @param pos Pointer to the Position structure to be modified.
 */
void moveUp(Position* pos);

/**
 * @brief Moves the object down, increasing the y value.
 * 
 * @param pos Pointer to the Position structure to be modified.
 */
void moveDown(Position* pos);

/**
 * @brief Moves the object to the left, decreasing the x value.
 * 
 * @param pos Pointer to the Position structure to be modified.
 */
void moveLeft(Position* pos);

/**
 * @brief Moves the object to the right, increasing the x value.
 * 
 * @param pos Pointer to the Position structure to be modified.
 */
void moveRight(Position* pos);

#endif // !__PLAYER_H__
