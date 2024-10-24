#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL2/SDL.h>

#define NUM_FRAMES 4

/**
 * @struct Sprite
 * @brief Represents a sprite entity in the game.
 * 
 * This structure holds the properties of a sprite, including its texture,
 * frames, and current frame index.
 * 
 * @param texture
 * The SDL_Texture pointer that represents the sprite's image.
 * 
 * @param frames
 * An array of SDL_Rect structures representing the sprite's frames.
 * 
 * @param current_frame
 * The index of the current frame being displayed.
 */
typedef struct {
    SDL_Texture *texture; 
    SDL_Rect frames[NUM_FRAMES]; 
    int current_frame; 
    }Sprite;

/**
 * @brief Loads a texture from a specified file.
 *
 * This function takes a file path and an SDL_Renderer pointer, and returns
 * an SDL_Texture pointer that represents the loaded texture. If the texture
 * cannot be loaded, the function returns NULL.
 *
 * @param file The path to the image file to load.
 * @param renderer The SDL_Renderer used to render the texture.
 * @return SDL_Texture* The loaded texture, or NULL if loading fails.
 */
SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer);

/**
 * @brief Renders a sprite on the screen.
 *
 * This function renders a sprite using the specified texture, source rectangle,
 * destination rectangle, and renderer.
 *
 * @param texture A pointer to the SDL_Texture to be rendered.
 * @param src A pointer to the SDL_Rect that defines the portion of the texture to be rendered.
 *            If NULL, the entire texture will be rendered.
 * @param dest A pointer to the SDL_Rect that defines the destination rectangle on the screen
 *             where the texture will be rendered.
 * @param renderer A pointer to the SDL_Renderer used for rendering the texture.
 */
void renderSprite(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest, SDL_Renderer* renderer);

/**
 * @brief Updates the current frame of the given sprite.
 *
 * This function advances the sprite's animation to the next frame.
 * It should be called periodically to animate the sprite correctly.
 *
 * @param sprite A pointer to the Sprite structure to be updated.
 */
void updateSpriteFrame(Sprite* sprite);


#endif // DEBUG