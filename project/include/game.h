#ifndef __GAME_H__
#define __GAME_H__
#include <SDL2/SDL.h>
#include <stdbool.h>
#define GRAVIDADE 250.0
#define DELTA_TIME 0.09
/**
 * @brief Initializes the game.
 *
 * This function sets up the initial state of the game, including
 * any necessary configurations, resource allocations, and initial
 * game settings. It should be called before starting the game loop.
 */
void initGame(SDL_Renderer* renderer);

/**
 * @brief Main loop of the game.
 *
 * This function contains the main game loop which handles the game's
 * core logic, including updating game state, processing user input,
 * and rendering the game. It runs continuously until the game is
 * terminated.
 */
void gameLoop(SDL_Renderer* renderer);

/**
 * @brief Shuts down the game and performs necessary cleanup operations.
 *
 * This function is responsible for gracefully shutting down the game,
 * releasing any allocated resources, and ensuring that the game state
 * is properly saved or terminated.
 */
void shutdownGame();

#endif // !__GAME_H__
