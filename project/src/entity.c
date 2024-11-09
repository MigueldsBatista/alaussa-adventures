
#include "entity.h"
#include "map.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "entity.h"
#include "game.h"

void freeAnimationFrames(Entity *entity) {
    for (int i = 0; i < entity->totalFrames; i++) {
        if (entity->animationFrames[i] != NULL) {
            SDL_DestroyTexture(entity->animationFrames[i]);
        }
    }
    free(entity->animationFrames);
    entity->animationFrames = NULL;
}

void initEntity(Entity *entity, Label label, int posX, int posY, int life_quantity, SDL_Renderer *renderer) {
    entity->position.x = posX;
    entity->position.y = posY;
    entity->position.velX = 0.0;
    entity->position.velY = 0.0;
    entity->position.onGround = true;
    entity->width = 64;
    entity->height = 64;
    entity->currentFrame = 0;
    entity->animationFrames = NULL; // Inicializa o ponteiro como NULL
    entity->label = label;
    entity->isAlive = true;
    initLifeEntity(entity, life_quantity);
    loadAnimationFrames(entity, IDLE, renderer);
}

void initLifeEntity(Entity *entity, int life_quantity) {
    Life *vida;
    entity->head = NULL;
    for (int i = 0; i < life_quantity; i++) {
        vida = (Life *)malloc(sizeof(Life));
        vida->id = i + 1;               
        vida->prox = entity->head;    
        entity->head = vida;          
    }
}

void damageEntity(Entity *entity) {
    Life *temp = entity->head;
    if(entity->head->prox == NULL){
        printf("Player morreu\n");
        entity->isAlive = false;
        return;
    }
    entity->head = entity->head->prox;
    free(temp);
}


void updateEntity(Entity *entity, SDL_Renderer *renderer) {
    // Aplica a gravidade à velocidade vertical

    entity->position.velY += GRAVIDADE * DELTA_TIME;

    // Limita a velocidade de queda
    if (entity->position.velY > 100) {
        entity->position.velY = 100;
    }

    if (entity->position.velX > 150) {
        entity->position.velX = 150;
    }

        if (entity->position.velX < -150) {
        entity->position.velX = -150;
    }


    // Atualiza a posição do jogador
    entity->position.x += (entity->position.velX * DELTA_TIME);
    entity->position.y += (entity->position.velY * DELTA_TIME);

    // Verifica a colisão com o bloco e atualiza `onGround`
    if (checkEntityBlockCollision(entity)) {
        entity->position.velY = 0; // Reseta a velocidade vertical ao colidir com o bloco
        entity->position.onGround = true; // Define que o jogador está no chão
    }

    // Define a animação correta com base no estado e velocidade
    if (entity->position.onGround) {
        if(entity->position.velX == 0 && entity->position.velY == 0){
            entity->currentFrame = 0;
            freeAnimationFrames(entity);
            loadAnimationFrames(entity, IDLE, renderer);
        }
        else if (entity->position.velX > 0 && entity->position.velY == 0) {
            // Movimento à direita no chão
            entity->currentFrame = (entity->currentFrame + 1) % entity->totalFrames;
            freeAnimationFrames(entity);
            loadAnimationFrames(entity, MOVE_RIGHT, renderer);
        }
        else if (entity->position.velX < 0 && entity->position.velY == 0) {
            // Movimento à esquerda no chão
            entity->currentFrame = (entity->currentFrame + 1) % entity->totalFrames;
            freeAnimationFrames(entity);
            loadAnimationFrames(entity, MOVE_LEFT, renderer);
        }
    }
    else if (!entity->position.onGround) {
        // Se está no ar, define a animação de pulo
        entity->currentFrame = (entity->currentFrame + 1) % entity->totalFrames;
        freeAnimationFrames(entity);
        if (entity->position.velX >= 0) {
            loadAnimationFrames(entity, JUMP_RIGHT, renderer);
        } else {
            loadAnimationFrames(entity, JUMP_LEFT, renderer);
        }
    }
}


void renderEntity(Entity *entity, SDL_Renderer *renderer) {
    SDL_Rect dstRect = {
        .x = (int)entity->position.x,
        .y = (int)entity->position.y,
        .w = entity->width,
        .h = entity->height
    };
    SDL_RenderCopy(renderer, entity->animationFrames[entity->currentFrame], NULL, &dstRect);
}

Action handleEntityInput(SDL_Event *event, Entity *entity) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
                moveLeft(entity);
                return MOVE_LEFT;
            case SDLK_d:
            case SDLK_RIGHT:
                moveRight(entity);
                return MOVE_RIGHT;
            case SDLK_w:
            case SDLK_SPACE:
            case SDLK_UP:
                jumpEntity(entity);
                return (entity->position.velX >= 0) ? JUMP_RIGHT : JUMP_LEFT;
        }
    } else if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
            case SDLK_LEFT:
            case SDLK_d:
            case SDLK_RIGHT:
                entity->position.velX = 0.0;
                return IDLE;
        }
    }
    return IDLE;
}

void loadAnimationFrames(Entity *entity, Action action, SDL_Renderer *renderer) {
    int frameCount = 0;
    
    // Determina o número de frames com base na ação
    if (entity->isAlive == false) {
        frameCount = 1;
    } else {
        switch (action) {
            case IDLE: frameCount = 1; break;
            case MOVE_LEFT:
            case MOVE_RIGHT: frameCount = 3; break;
            case JUMP_RIGHT:
            case JUMP_LEFT:
            case FALL_LEFT:
            case FALL_RIGHT: frameCount = 3; break;
            default: break;
        }
    }

    // Aloca memória para as texturas de animação
    entity->animationFrames = malloc(sizeof(SDL_Texture*) * frameCount);
    entity->totalFrames = frameCount;

    // Carrega as animações com base no tipo (jogador ou inimigo)
    for (int i = 0; i < frameCount; i++) {
        char filename[100]; // Para armazenar o caminho da imagem
        SDL_Surface *surface = NULL;  // Superfície temporária para carregar a imagem

        // Verifica se o entity é um jogador ou inimigo
        if (entity->label == PLAYER) {
            // Player animations
            if (action == MOVE_LEFT) {
                sprintf(filename, "project/assets/MovPlayer/player_left_%d.png", i);
            } else if (action == MOVE_RIGHT) {
                sprintf(filename, "project/assets/MovPlayer/player_right_%d.png", i);
            } else if (action == IDLE) {
                sprintf(filename, "project/assets/MovPlayer/player_idle_%d.png", i);
            } else if (action == JUMP_LEFT) {
                sprintf(filename, "project/assets/MovPlayer/player_jump_left_%d.png", i);
            } else if (action == JUMP_RIGHT) {
                sprintf(filename, "project/assets/MovPlayer/player_jump_right_%d.png", i);
            }
        } else if (entity->label == ENEMY) {
            // Enemy animations
            if (action == MOVE_LEFT) {
                sprintf(filename, "project/assets/MovEnemy/enemy_move_left_%d.png", i);
            } else if (action == MOVE_RIGHT) {
                sprintf(filename, "project/assets/MovEnemy/enemy_move_right_%d.png", i);
            } else if (action == IDLE) {
                sprintf(filename, "project/assets/MovEnemy/enemy_idle_%d.png", i);
            }
        }
        else if(entity->isAlive == false){
            sprintf(filename, "project/assets/MovPlayer/player_gameover_%d.png", i);
        }
        // Carrega a imagem da textura
        surface = IMG_Load(filename);
        if (surface == NULL) {
            printf("Falha ao carregar a imagem: %s\n", IMG_GetError());
            entity->animationFrames[i] = NULL;
        } else {
            entity->animationFrames[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
}
void jumpEntity(Entity *entity) {
    if (entity->position.onGround) {
        entity->position.velY = -260.0;
        entity->position.onGround = false;
    }
}

void moveLeft(Entity *entity) {
    entity->position.velX = -100.0;
}

void moveRight(Entity *entity) {
    entity->position.velX = 100.0;
}
