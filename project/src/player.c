#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "player.h"


void initPlayer(Player *player) {
    player->position.x = 100; // Posição inicial
    player->position.y = 500; // Altura inicial (exemplo)
    player->position.velX = 0;
    player->position.velY = 0;
    player->position.onGround = true; // Começa no chão
    player->width = 32;  // Exemplo: largura do sprite do Mario
    player->height = 32; // Exemplo: altura do sprite do Mario
}

void updatePlayer(Player *player, double gravity, double deltaTime) {
    if (!player->position.onGround) {
        player->position.velY += gravity * deltaTime; // Aplicar gravidade
        player->position.y += player->position.velY * deltaTime;

        // Verifica se colidiu com o chão (exemplo: y=500 é o nível do chão)
        if (player->position.y >= 500) {
            player->position.y = 500; // Ajusta para não passar do chão
            player->position.velY = 0;
            player->position.onGround = true; // Está no chão
        }
    }
}

void renderPlayer(Player *player) {
    // Aqui você deve implementar a renderização do sprite do jogador usando SDL
    // Exemplo:
    // SDL_Rect rect = { (int)player->position.x, (int)player->position.y, player->width, player->height };
    // SDL_RenderCopy(renderer, playerTexture, NULL, &rect);
}

void AgacharPlayer(Position *pos) {
    pos->Agachado = true;
    pos->y += 10; // Mova a posição para baixo (ou ajuste a altura conforme necessário)
}

void LevantarPlayer(Position *pos) {
    pos->Agachado = false;
    pos->y -= 10; // Volte para a posição original
}

void handlePlayerInput(SDL_Event *event, Player *player) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_a: 
                moveLeft(player);
                break;
            case SDLK_d: 
                moveRight(player);
                break;
            case SDLK_w: // Faz pular
                jumpPlayer(player);
                break;
            case SDLK_s: 
                // Implementar lógica de agachar se necessário
                break;
            case SDLK_LEFT: // Flecha esquerda
                moveLeft(player);
                break;
            case SDLK_RIGHT: // Flecha direita
                moveRight(player);
                break;
            case SDLK_SPACE: // Pulo
                jumpPlayer(player);
                break;
        }
    }
}


void jumpPlayer(Player *player) {
    if (player->position.onGround) {
        player->position.velY = -400; // Impulso para o pulo
        player->position.onGround = false; // Não está mais no chão
    }
}

void moveLeft(Player *player) {
    player->position.velX = -200; // Velocidade de movimento para a esquerda
    player->position.x += player->position.velX; // Atualiza a posição
}

void moveRight(Player *player) {
    player->position.velX = 200; // Velocidade de movimento para a direita
    player->position.x += player->position.velX; // Atualiza a posição
}
