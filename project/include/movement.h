#ifndef MOVEMENT_H
#define MOVEMENT_H

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

#endif // MOVIMENTO_H
