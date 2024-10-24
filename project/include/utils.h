#ifndef __UTILS_H__
#define __UTILS_H__


#include <SDL2/SDL.h>

/**
 * @brief Loads an image from a file and creates an SDL_Texture.
 *
 * This function takes a filename and an SDL_Renderer, loads the image from the specified file,
 * and creates an SDL_Texture that can be used for rendering.
 *
 * @param filename The path to the image file to be loaded.
 * @param renderer The SDL_Renderer to be used for creating the texture.
 * @return A pointer to the created SDL_Texture, or NULL if the image could not be loaded.
 */
SDL_Texture* loadImage(const char* filename, SDL_Renderer* renderer);

/**
 * @brief Introduces a delay in the program execution.
 *
 * This function pauses the execution of the program for a duration specified
 * by the frame_limit parameter. The delay is typically used to control the 
 * frame rate in games or other time-sensitive applications.
 *
 * @param frame_limit The number of frames to delay the execution.
 */
void delay(unsigned int frame_limit);
#endif // !__UTILS_H__

#include <SDL2/SDL.h>
