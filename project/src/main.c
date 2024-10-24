#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "map.h"
#include "player.h"
#include "enemy.h"
#include "sprite.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Error initializing SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a window and renderer
    SDL_Window* window = SDL_CreateWindow("Background Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load the background texture
    SDL_Texture* background = loadTexture("project/assets/images/parallax.png", renderer);

    // Game loop
    int running = 1;
    int camera_position = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Render the background with parallax effect
        renderBackground(renderer, background, camera_position, 640, 480);

        // Present the screen
        SDL_RenderPresent(renderer);

        // Simulate camera movement
        camera_position += 1;

        // Limit the frame rate
        SDL_Delay(16);
    }

    // Clean up resources
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}