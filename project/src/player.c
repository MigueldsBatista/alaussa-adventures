#include "entity.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

void renderPlayerLife(Entity *player, SDL_Renderer *renderer, TTF_Font *font) {
    if (player->head != NULL) { 
        char lifeText[10];
        sprintf(lifeText, "Life %d", player->head->id);
        SDL_Surface *surface = TTF_RenderText_Solid(font, lifeText, (SDL_Color){255, 255, 255});
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dstrect = {10, 10, surface->w, surface->h}; 
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void renderPlayerCoins(Entity *player, SDL_Renderer *renderer, TTF_Font *font){
    char coinText[16];
    if(player->head != NULL){
    snprintf(coinText, sizeof(coinText), "Moedas: %d", player->moedas);
    SDL_Surface *surface = TTF_RenderText_Solid(font, coinText, (SDL_Color){255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstrect = {10, 40, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    }
}

#include <time.h>

void writePlayerInfo(Entity *player, const char* playerName) {
    // Abrindo o arquivo em modo de anexação ("a")
    FILE *file = fopen("ranking.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de ranking\n");
        return;
    }

    // Obtendo a data e hora atual
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char dateTime[100];
    strftime(dateTime, sizeof(dateTime)-1, "%Y-%m-%d %H:%M:%S", t);

    // Escrevendo as informações do jogador no arquivo
    fprintf(file, "Jogador: %s | Pontos: %d | Data e Hora: %s\n", playerName, player->moedas, dateTime);

    // Fechando o arquivo
    fclose(file);
}