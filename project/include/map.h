#ifndef __MAP_H__
#define __MAP_H__

#include <SDL2/SDL.h>

/**
 * @brief Represents a block in the game map.
 * 
 * This structure defines a block with a specific type and its position and size
 * represented by an SDL_Rect structure.
 */
typedef struct {
     int type; 
     SDL_Rect rectangle; 
}Block;

/**
 * @brief Loads a map from a specified file.
 *
 * This function reads the map data from the given file and initializes
 * the map structure accordingly.
 *
 * @param map_file The path to the map file to be loaded.
 */
void loadMap(const char* map_file);

/**
 * @brief Renders the map on the screen.
 *
 * This function is responsible for drawing the map in the game. It handles
 * all the necessary operations to display the current state of the map
 * to the player.
 */
void renderMap();

#endif // !__MAP_H__
