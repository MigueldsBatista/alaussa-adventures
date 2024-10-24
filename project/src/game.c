#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "sprite.h"
#include "map.h"

void initGame(SDL_Renderer* renderer) {
    //TODO: Implementar a inicialização do jogo
    //initEnemy(); // Inicializa inimigos
    loadMap("project/assets/images/parallax.png"); // Carrega o mapa
}



void gameLoop(SDL_Renderer* renderer){
    //TODO: Implementar o loop do jogo
}

void shutdownGame(){
//TODO: Implementar a finalização do jogo    
return;
}