#ifndef __ENTITY_H__
#define __ENTITY_H__  // Define ENTITY_H


#include <SDL2/SDL.h>
#include <stdbool.h>

//une a estrutura de dados do entity e do inimigo

/**
 * @brief Structure that represents the position of the entity in the game.
 * 
 * The Position structure stores the x and y coordinates, velocity, 
 * and flags to indicate the entity's state (e.g., whether on the ground).
 */
typedef struct Position{
    double x, y;       // Position of the entity
    double velX, velY; // Velocities in the x and y axes
    bool onGround;     // Indicates if the entity is on the ground
} Position;

typedef enum {
    IDLE,
    MOVE_LEFT,
    MOVE_RIGHT,
    JUMP_LEFT,
    JUMP_RIGHT,
    FALL_LEFT,
    FALL_RIGHT,
} Action;

typedef enum{
    PLAYER,
    ENEMY
}Label;

typedef struct Life{
    int id;
    struct Life *prox;
} Life;

/**
 * @brief Structure that represents the entity.
 * 
 * This structure includes the entity's position, dimensions, and 
 * animation properties.
 */
typedef struct {
    SDL_Texture **animationFrames;
    int totalFrames;
    int currentFrame;
    Position position;             // Posição do jogador
    int width;
    int height;
    Action currentAction;
    Life *head; // head da vida do entity
    Label label;
} Entity;

/**
 * @brief Initializes the entity.
 * 
 * This function sets up the initial state of the entity, including
 * necessary variables and configurations required for the entity to function correctly.
 * 
 * @param entity Pointer to the Entity structure to initialize.
 */

void initEntity(Entity *entity, Label label, int posX, int posY, int life_quantity, SDL_Renderer *renderer);

/**
 * @brief Updates the entity's state.
 *
 * This function updates the entity's position, handles gravity,
 * and updates the animation state based on the entity's current action.
 * 
 * @param entity Pointer to the Entity structure to update.
 * @param gravity The force of gravity to apply.
 * @param deltaTime Time since the last update (in seconds).
 */
void updateEntity(Entity *entity, SDL_Renderer *renderer);

/**
 * @brief Renders the entity on the screen.
 *
 * This function is responsible for drawing the current frame of the entity character
 * on the screen based on its current position and state.
 * 
 * @param entity Pointer to the Entity structure to render.
 * @param renderer Pointer to the SDL_Renderer for drawing.
 */
void renderEntity(Entity *entity, SDL_Renderer *renderer);

/**
 * @brief Handles the entity's input events.
 * 
 * This function processes the SDL_Event passed to it and updates the entity's
 * state accordingly based on the input received.
 * 
 * @param event Pointer to an SDL_Event structure that contains the event data.
 * @param entity Pointer to the Entity structure to update.
 */

Action handleEntityInput(SDL_Event *event, Entity *entity);

/**
 * @brief Makes the entity jump.
 * 
 * This function applies an upward velocity to the entity, causing them to jump.
 * 
 * @param entity Pointer to the Entity structure.
 */
void jumpEntity(Entity *entity);

/**
 * @brief Moves the entity left.
 * 
 * This function modifies the entity's position to move left.
 * 
 * @param entity Pointer to the Entity structure.
 */
void moveLeft(Entity *entity);

/**
 * @brief Moves the entity right.
 * 
 * This function modifies the entity's position to move right.
 * 
 * @param entity Pointer to the Entity structure.
 */
void moveRight(Entity *entity);

/**
 * @brief Crouches the entity.
 * 
 * This function modifies the entity's state to indicate that they are crouching.
 * 
 * @param pos Pointer to the Position structure of the entity.
 */
void AgacharEntity(Position *pos);

/**
 * @brief Makes the entity stand up.
 * 
 * This function modifies the entity's state to indicate that they are standing.
 * 
 * @param pos Pointer to the Position structure of the entity.
 */
void LevantarEntity(Position *pos);

/**
 * @brief Updates the entity's animation based on their current action.
 * 
 * This function updates the current frame of the entity's animation based 
 * on the time elapsed and the entity's action.
 * 
 * @param entity Pointer to the Entity structure to update.
 * @param action The current action of the entity (e.g., idle, moving).
 * @param deltaTime Time since the last update (in seconds).
 */
void updateEntityAnimation(Entity *entity, Action action, double deltaTime);

/**
 * @brief Loads the animation frames for the entity.
 * 
 * This function loads the texture frames for the specified action and 
 * stores them in the entity's animationFrames array.
 * 
 * @param entity Pointer to the Entity structure to load frames into.
 * @param action The action for which to load frames.
 * @param renderer Pointer to the SDL_Renderer for loading textures.
 */
void loadAnimationFrames(Entity *entity, Action action, SDL_Renderer *renderer);

void initLifeEntity(Entity *entity, int life_quantity);

void damageEntity(Entity *entity);

#endif // !__ENTITY_H__
