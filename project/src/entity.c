
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

void initEnemy(Entity *entity, Label label, int posX, int posY, int life_quantity, SDL_Renderer *renderer) {
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
    initLifeEntity(entity, life_quantity);
    loadAnimationFrames(entity, IDLE, renderer);
}

void initPlayer(Entity *entity, Label label, int posX, int posY, int life_quantity, SDL_Renderer *renderer) {
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
    entity->moedas=0;
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
    // Se o jogador já está imune, não faz nada
    if (entity->imortalidadeAtiva) {
        return;
    }
    // Retira uma vida
    Life *temp = entity->head;
    if (entity->head->prox == NULL) {
        printf("Player morreu\n");
        entity->isAlive = false;
        return;
    }
    entity->head = entity->head->prox;
    free(temp);
    // Ativa a imortalidade
    entity->imortalidadeAtiva = true;
    entity->imortalidadeTimer = 2.0f;  // 2 segundos de imortalidade (ajuste conforme necessário)
}


void updateEntity(Entity *entity, SDL_Renderer *renderer) {
    // Atualiza a física
    entity->position.velY += GRAVIDADE * DELTA_TIME;

    if (entity->position.velY > 100) {
        entity->position.velY = 100;
    }

    if (entity->position.velX > 150) {
        entity->position.velX = 150;
    } else if (entity->position.velX < -150) {
        entity->position.velX = -150;
    }

    entity->position.x += (entity->position.velX * DELTA_TIME);
    entity->position.y += (entity->position.velY * DELTA_TIME);

    // Checa colisão com blocos
    if (checkEntityBlockCollision(entity)) {
        entity->position.velY = 0;
        entity->position.onGround = true;
    }

    // Define o estado atual
    Action newAction;
    if (entity->imortalidadeAtiva && entity->label == PLAYER) {
        newAction = TAKING_DAMAGE;
    }
    
    else if (entity->position.onGround) {
        if (entity->position.velX == 0) {
            newAction = IDLE;
        } else if (entity->position.velX > 0) {
            newAction = MOVE_RIGHT;
        } else {
            newAction = MOVE_LEFT;
        }
    } else {
        if (entity->position.velX >= 0) {
            newAction = JUMP_RIGHT;
        } else {
            newAction = JUMP_LEFT;
        }
    }

    // Carrega a animação apenas se o estado mudar
    if (entity->currentAction != newAction) {
        entity->currentAction = newAction;
        freeAnimationFrames(entity); // Libera os quadros anteriores
        loadAnimationFrames(entity, newAction, renderer);
        entity->currentFrame = 0; // Reinicia o quadro da animação
    }
    /*

    currentFrame	Após incremento	Cálculo % totalFrames	Novo currentFrame
            0	            1	            1 % 4 → 1	            1
            1	            2	            2 % 4 → 2	            2
            2	            3	            3 % 4 → 3	            3
            3	            4	            4 % 4 → 0	            0 (reinicia)
    
    */
    // Atualiza o quadro da animação
    entity->currentFrame = (entity->currentFrame + 1) % entity->totalFrames;
}

void renderEntity(Entity *entity, SDL_Renderer *renderer) {
    if (entity->currentFrame >= entity->totalFrames || entity->animationFrames == NULL) {
        return;
    }

    SDL_Rect dstRect = {
        .x = (int)entity->position.x,
        .y = (int)entity->position.y,
        .w = entity->width,
        .h = entity->height
    };

    SDL_Texture *currentTexture = entity->animationFrames[entity->currentFrame];
    if (currentTexture != NULL) {
        SDL_RenderCopy(renderer, currentTexture, NULL, &dstRect);
    }
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
    

        switch (action) {
            case IDLE: 
            case TAKING_DAMAGE: frameCount = 1;
            break;

            case MOVE_LEFT:
            case MOVE_RIGHT:
                frameCount = 3;
                break;

            case JUMP_RIGHT:
            case JUMP_LEFT:
            case FALL_LEFT:
            case FALL_RIGHT: frameCount = 3; break;
            default: break;
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
            else if(action == TAKING_DAMAGE) {
                sprintf(filename, "project/assets/MovPlayer/player_damage.png");
            }

        }
        else if (entity->label == ENEMY) {
            // Enemy animations
            if (action == MOVE_LEFT) {
                sprintf(filename, "project/assets/MovEnemy/enemy_move_left_%d.png", i);
            } else if (action == MOVE_RIGHT) {
                sprintf(filename, "project/assets/MovEnemy/enemy_move_right_%d.png", i);
            }// else if (action == IDLE) {
             //   sprintf(filename, "project/assets/MovEnemy/enemy_idle_%d.png", i);
            //}
            
            else{
                sprintf(filename, "project/assets/MovEnemy/enemy_idle_%d.png", i);
            }
        }

        // Carrega a imagem da textura
        surface = IMG_Load(filename);
        if (surface == NULL) {
            //printf("Falha ao carregar a imagem: %s, SDL_Error: %s\n", filename, SDL_GetError());
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

void atualizarImortalidade(Entity *entity, float deltaTime) {
    if (entity->imortalidadeAtiva) {
        entity->imortalidadeTimer -= deltaTime;

        if (entity->imortalidadeTimer <= 0) {
            entity->imortalidadeAtiva = false;
        }
    }
}