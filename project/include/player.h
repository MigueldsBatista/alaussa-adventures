#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <SDL2/SDL.h>

/**
 * @brief Estrutura que representa a posição de um objeto ou personagem no jogo.
 * 
 * A estrutura Posicao armazena as coordenadas x e y, onde:
 * - x: Coordenada horizontal.
 * - y: Coordenada vertical.
 */
typedef struct {
    int x; ///< Coordenada horizontal (posição x).
    int y; ///< Coordenada vertical (posição y).
} Posicao;

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
 * @brief Move o objeto para cima, diminuindo o valor de y.
 * 
 * @param pos Ponteiro para a estrutura Posicao a ser alterada.
 */
void mover_para_cima(Posicao* pos);

/**
 * @brief Move o objeto para baixo, aumentando o valor de y.
 * 
 * @param pos Ponteiro para a estrutura Posicao a ser alterada.
 */
void mover_para_baixo(Posicao* pos);

/**
 * @brief Move o objeto para a esquerda, diminuindo o valor de x.
 * 
 * @param pos Ponteiro para a estrutura Posicao a ser alterada.
 */
void mover_para_esquerda(Posicao* pos);

/**
 * @brief Move o objeto para a direita, aumentando o valor de x.
 * 
 * @param pos Ponteiro para a estrutura Posicao a ser alterada.
 */
void mover_para_direita(Posicao* pos);

#endif // !__PLAYER_H__