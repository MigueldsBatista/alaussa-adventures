#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Structure that represents the position of the player in the game.
 * 
 * The Position structure stores the x and y coordinates, as well as the velocity.
 */
typedef struct {
    double x, y;       // Posição do jogador
    double velX, velY; // Velocidades em x e y
    bool onGround;
    bool Agachado;     // Flag que indica se o jogador está no chão
} Position;

/**
 * @brief Structure that represents the player.
 * 
 * This structure includes the player's position and any other relevant attributes.
 */
typedef struct {
    Position position; // Posição do jogador
    int width;        // Largura do sprite do jogador
    int height;      // Altura do sprite do jogador
} Player;

/**
 * @brief Initializes the player.
 * 
 * This function sets up the initial state of the player, including
 * necessary variables and configurations required for the player to function correctly.
 */
void initPlayer(Player *player);

/**
 * @brief Updates the player's state.
 *
 * This function updates the player's position and handles gravity.
 */
void updatePlayer(Player *player, double gravity, double deltaTime);

/**
 * @brief Renders the player on the screen.
 *
 * This function is responsible for drawing the player character
 * on the screen.
 */
void renderPlayer(Player *player);

/**
 * @brief Handles the player's input events.
 * 
 * This function processes the SDL_Event passed to it and updates the player's
 * state accordingly based on the input received.
 * 
 * @param event Pointer to an SDL_Event structure that contains the event data.
 * @param player Pointer to the Player structure to update.
 */
void handlePlayerInput(SDL_Event *event, Player *player);

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

void AgacharPlayer(Position *pos);

void LevantarPlayer(Position *pos);

#endif // !__PLAYER_H__
