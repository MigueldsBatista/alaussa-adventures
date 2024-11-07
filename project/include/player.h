#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Structure that represents the position of the player in the game.
 * 
 * The Position structure stores the x and y coordinates, velocity, 
 * and flags to indicate the player's state (e.g., whether on the ground).
 */
typedef struct {
    double x, y;       // Position of the player
    double velX, velY; // Velocities in the x and y axes
    bool onGround;     // Indicates if the player is on the ground
} Position;

typedef enum {
    PLAYER_IDLE,// Estado de repouso
    PLAYER_MOVE_LEFT,
    PLAYER_MOVE_RIGHT,
    PLAYER_JUMP_LEFT,
    PLAYER_JUMP_RIGHT,
    PLAYER_FALL_LEFT,
    PLAYER_FALL_RIGHT,
} PlayerAction;


typedef struct Life{
    int id;
    struct Life *prox;
} Life;



/**
 * @brief Structure that represents the player.
 * 
 * This structure includes the player's position, dimensions, and 
 * animation properties.
 */
typedef struct {
    SDL_Texture **animationFrames;
    int totalFrames;
    int currentFrame;
    Position position;             // Posição do jogador
    int width;
    int height;
    PlayerAction currentAction;
    Life *Becker; // head da vida do player
} Player;

/**
 * @brief Initializes the player.
 * 
 * This function sets up the initial state of the player, including
 * necessary variables and configurations required for the player to function correctly.
 * 
 * @param player Pointer to the Player structure to initialize.
 */

void initPlayer(Player *player, SDL_Renderer *renderer);

/**
 * @brief Updates the player's state.
 *
 * This function updates the player's position, handles gravity,
 * and updates the animation state based on the player's current action.
 * 
 * @param player Pointer to the Player structure to update.
 * @param gravity The force of gravity to apply.
 * @param deltaTime Time since the last update (in seconds).
 */
void updatePlayer(Player *player, double gravity, double deltaTime, SDL_Renderer *renderer);

/**
 * @brief Renders the player on the screen.
 *
 * This function is responsible for drawing the current frame of the player character
 * on the screen based on its current position and state.
 * 
 * @param player Pointer to the Player structure to render.
 * @param renderer Pointer to the SDL_Renderer for drawing.
 */
void renderPlayer(Player *player, SDL_Renderer *renderer);

/**
 * @brief Handles the player's input events.
 * 
 * This function processes the SDL_Event passed to it and updates the player's
 * state accordingly based on the input received.
 * 
 * @param event Pointer to an SDL_Event structure that contains the event data.
 * @param player Pointer to the Player structure to update.
 */
PlayerAction handlePlayerInput(SDL_Event *event, Player *player);

/**
 * @brief Makes the player jump.
 * 
 * This function applies an upward velocity to the player, causing them to jump.
 * 
 * @param player Pointer to the Player structure.
 */
void jumpPlayer(Player *player);

/**
 * @brief Moves the player left.
 * 
 * This function modifies the player's position to move left.
 * 
 * @param player Pointer to the Player structure.
 */
void moveLeft(Player *player);

/**
 * @brief Moves the player right.
 * 
 * This function modifies the player's position to move right.
 * 
 * @param player Pointer to the Player structure.
 */
void moveRight(Player *player);

/**
 * @brief Crouches the player.
 * 
 * This function modifies the player's state to indicate that they are crouching.
 * 
 * @param pos Pointer to the Position structure of the player.
 */
void AgacharPlayer(Position *pos);

/**
 * @brief Makes the player stand up.
 * 
 * This function modifies the player's state to indicate that they are standing.
 * 
 * @param pos Pointer to the Position structure of the player.
 */
void LevantarPlayer(Position *pos);

/**
 * @brief Updates the player's animation based on their current action.
 * 
 * This function updates the current frame of the player's animation based 
 * on the time elapsed and the player's action.
 * 
 * @param player Pointer to the Player structure to update.
 * @param action The current action of the player (e.g., idle, moving).
 * @param deltaTime Time since the last update (in seconds).
 */
void updatePlayerAnimation(Player *player, PlayerAction action, double deltaTime);

/**
 * @brief Loads the animation frames for the player.
 * 
 * This function loads the texture frames for the specified action and 
 * stores them in the player's animationFrames array.
 * 
 * @param player Pointer to the Player structure to load frames into.
 * @param action The action for which to load frames.
 * @param renderer Pointer to the SDL_Renderer for loading textures.
 */
void loadAnimationFrames(Player *player, PlayerAction action, SDL_Renderer *renderer);

void initLifePlayer(Player *player);

void damagePlayer(Player *player);

#endif // !__PLAYER_H__
