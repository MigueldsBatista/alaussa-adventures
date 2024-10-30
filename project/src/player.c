#include "player.h"
#include <SDL2/SDL_image.h>

void initPlayer(Player *player, SDL_Renderer *renderer) {
    player->position.x = 100.0;
    player->position.y = 100.0;
    player->position.velX = 0.0;
    player->position.velY = 0.0;
    player->position.onGround = true;
    player->width = 64;
    player->height = 64;
    player->currentFrame = 0;
    loadAnimationFrames(player, PLAYER_IDLE, renderer);
}

void updatePlayer(Player *player, double gravity, double deltaTime, SDL_Renderer *renderer) {
    // Aplicar gravidade se o jogador não estiver no chão
    if (!player->position.onGround) {
        player->position.velY += gravity * deltaTime;
    }

    // Atualizar a posição do jogador
    player->position.x += player->position.velX * deltaTime;
    player->position.y += player->position.velY * deltaTime;

    // Atualizar a animação com base na posição e no estado
    if (player->position.velX > 0) { // Movendo para a direita
        player->currentFrame = (player->currentFrame + 1) % player->totalFrames;
        loadAnimationFrames(player, PLAYER_MOVE_RIGHT, renderer); // Carregar frames de movimento para a direita
    } else if (player->position.velX < 0) { // Movendo para a esquerda
        player->currentFrame = (player->currentFrame + 1) % player->totalFrames;
        loadAnimationFrames(player, PLAYER_MOVE_LEFT, renderer); // Carregar frames de movimento para a esquerda
    } else { // Jogador está parado
        player->currentFrame = 0; // Reset para o frame de idle
        loadAnimationFrames(player, PLAYER_IDLE, renderer); // Carregar frames de idle
    }
}

void renderPlayer(Player *player, SDL_Renderer *renderer) {
    SDL_Rect dstRect;
    dstRect.x = (int)player->position.x;
    dstRect.y = (int)player->position.y;
    dstRect.w = player->width;
    dstRect.h = player->height;

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
                jumpPlayer(player);
                return PLAYER_JUMP;
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
        case PLAYER_IDLE:
            frameCount = 1;
            break;
        case PLAYER_MOVE_LEFT:
        case PLAYER_MOVE_RIGHT:
            frameCount = 3;
            break;
        case PLAYER_JUMP:
        case PLAYER_FALL:
            frameCount = 2;
            break;
        default:
            break;
    }

    player->animationFrames = malloc(sizeof(SDL_Texture*) * frameCount);
    player->totalFrames = frameCount;

    for (int i = 0; i < frameCount; i++) {
        char filename[50];

        if (action == PLAYER_MOVE_LEFT) {
            sprintf(filename, "project/assets/images/player_left_%d.png", i);
        } else if (action == PLAYER_MOVE_RIGHT) {
            sprintf(filename, "project/assets/images/player_right_%d.png", i );
        } else if (action == PLAYER_IDLE) {
            sprintf(filename, "project/assets/images/player_idle_%d.png", i );
        } else if (action == PLAYER_JUMP) {
            sprintf(filename, "project/assets/images/player_jump_%d.png", i );
        }

        SDL_Surface *surface = IMG_Load(filename);
        if (surface == NULL) {
            printf("Falha ao carregar a imagem: %s\n", IMG_GetError());
        } else {
            player->animationFrames[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
}

void jumpPlayer(Player *player) {
    if(player->position.onGround){
    player->position.velY = -300.0;
    player->position.onGround = false;        
    }

}

void moveLeft(Player *player) {
    player->position.velX = -200.0;
}

void moveRight(Player *player) {
    player->position.velX = 200.0;
}
