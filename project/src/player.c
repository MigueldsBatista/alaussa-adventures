#include "player.h"
#include "map.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>

void freeAnimationFrames(Player *player) {
    for (int i = 0; i < player->totalFrames; i++) {
        if (player->animationFrames[i] != NULL) {
            SDL_DestroyTexture(player->animationFrames[i]);
        }
    }
    free(player->animationFrames);
    player->animationFrames = NULL;
}

void initPlayer(Player *player, SDL_Renderer *renderer) {
    player->position.x = 50.0;
    player->position.y = 50.0;
    player->position.velX = 0.0;
    player->position.velY = 0.0;
    player->position.onGround = true;
    player->width = 64;
    player->height = 64;
    player->currentFrame = 0;
    player->animationFrames = NULL; // Inicializa o ponteiro como NULL
    initLifePlayer(player);
    loadAnimationFrames(player, PLAYER_IDLE, renderer);
}

void initLifePlayer(Player *player) {
    Life *vida;
    player->Becker = NULL;
    for (int i = 0; i < 3; i++) {
        vida = (Life *)malloc(sizeof(Life));
        vida->id = i + 1;               
        vida->prox = player->Becker;    
        player->Becker = vida;          
    }
}

void damagePlayer(Player *player) {
    Life *temp = player->Becker;
    player->Becker = player->Becker->prox;
    free(temp);
}



void updatePlayer(Player *player, double gravity, double deltaTime, SDL_Renderer *renderer) {
    // Aplica a gravidade à velocidade vertical
    player->position.velY += gravity * deltaTime;

    // Limita a velocidade de queda
    if (player->position.velY > 120) {
        player->position.velY = 120;
    }

    if (player->position.velX > 150) {
        player->position.velX = 150;
    }

        if (player->position.velX < -150) {
        player->position.velX = -150;
    }


    // Atualiza a posição do jogador
    player->position.x += (player->position.velX * deltaTime);
    player->position.y += (player->position.velY * deltaTime);

    // Verifica a colisão com o bloco e atualiza `onGround`
    if (checkPlayerBlockCollision(player)) {
        player->position.velY = 0; // Reseta a velocidade vertical ao colidir com o bloco
        player->position.onGround = true; // Define que o jogador está no chão
    }

    // Define a animação correta com base no estado e velocidade
    if (player->position.onGround) {
        if(player->position.velX == 0 && player->position.velY == 0){
            player->currentFrame = 0;
            freeAnimationFrames(player);
            loadAnimationFrames(player, PLAYER_IDLE, renderer);
        }
        else if (player->position.velX > 0 && player->position.velY == 0) {
            // Movimento à direita no chão
            player->currentFrame = (player->currentFrame + 1) % player->totalFrames;
            freeAnimationFrames(player);
            loadAnimationFrames(player, PLAYER_MOVE_RIGHT, renderer);
        }
        else if (player->position.velX < 0 && player->position.velY == 0) {
            // Movimento à esquerda no chão
            player->currentFrame = (player->currentFrame + 1) % player->totalFrames;
            freeAnimationFrames(player);
            loadAnimationFrames(player, PLAYER_MOVE_LEFT, renderer);
        }
    }
    else if (!player->position.onGround) {
        // Se está no ar, define a animação de pulo
        player->currentFrame = (player->currentFrame + 1) % player->totalFrames;
        freeAnimationFrames(player);
        if (player->position.velX >= 0) {
            loadAnimationFrames(player, PLAYER_JUMP_RIGHT, renderer);
        } else {
            loadAnimationFrames(player, PLAYER_JUMP_LEFT, renderer);
        }
    }
}


void renderPlayer(Player *player, SDL_Renderer *renderer) {
    SDL_Rect dstRect = {
        .x = (int)player->position.x,
        .y = (int)player->position.y,
        .w = player->width,
        .h = player->height
    };
    SDL_RenderCopy(renderer, player->animationFrames[player->currentFrame], NULL, &dstRect);
}

PlayerAction handlePlayerInput(SDL_Event *event, Player *player) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
                moveLeft(player);
                return PLAYER_MOVE_LEFT;
            case SDLK_d:
            case SDLK_RIGHT:
                moveRight(player);
                return PLAYER_MOVE_RIGHT;
            case SDLK_w:
            case SDLK_SPACE:
            case SDLK_UP:
                jumpPlayer(player);
                return (player->position.velX >= 0) ? PLAYER_JUMP_RIGHT : PLAYER_JUMP_LEFT;
        }
    } else if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
            case SDLK_d:
            case SDLK_RIGHT:
                player->position.velX = 0.0;
                return PLAYER_IDLE;
        }
    }
    return PLAYER_IDLE;
}

void loadAnimationFrames(Player *player, PlayerAction action, SDL_Renderer *renderer) {
    int frameCount = 0;
    switch (action) {
        case PLAYER_IDLE: frameCount = 1; break;
        case PLAYER_MOVE_LEFT:
        case PLAYER_MOVE_RIGHT: frameCount = 3; break;
        case PLAYER_JUMP_RIGHT:
        case PLAYER_JUMP_LEFT:
        case PLAYER_FALL_LEFT:
        case PLAYER_FALL_RIGHT: frameCount = 3; break;
        default: break;
    }

    player->animationFrames = malloc(sizeof(SDL_Texture*) * frameCount);
    player->totalFrames = frameCount;

    for (int i = 0; i < frameCount; i++) {
        char filename[100];
        if (action == PLAYER_MOVE_LEFT) {
            sprintf(filename, "project/assets/MovPlayer/player_left_%d.png", i);
        } else if (action == PLAYER_MOVE_RIGHT) {
            sprintf(filename, "project/assets/MovPlayer/player_right_%d.png", i);
        } else if (action == PLAYER_IDLE) {
            sprintf(filename, "project/assets/MovPlayer/player_idle_%d.png", i);
        } else if (action == PLAYER_JUMP_LEFT) {
            sprintf(filename, "project/assets/MovPlayer/player_jump_left_%d.png", i);
        } else if (action == PLAYER_JUMP_RIGHT) {
            sprintf(filename, "project/assets/MovPlayer/player_jump_right_%d.png", i);
        }

        SDL_Surface *surface = IMG_Load(filename);
        if (surface == NULL) {
            printf("Falha ao carregar a imagem: %s\n", IMG_GetError());
            player->animationFrames[i] = NULL;
        } else {
            player->animationFrames[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
}

void jumpPlayer(Player *player) {
    if (player->position.onGround) {
        player->position.velY = -450.0;
        player->position.onGround = false;
    }
}

void moveLeft(Player *player) {
    player->position.velX = -100.0;
}

void moveRight(Player *player) {
    player->position.velX = 100.0;
}
