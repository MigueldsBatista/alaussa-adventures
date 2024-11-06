#include "sound.h"
#include <stdio.h>

int initAudio() {//inicia a biblioteca SDL então se aparecer erro nisso quer dizer q tu não baixou ainda :)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
        return 0;
    }
    printf("SDL_mixer inicializado com sucesso.\n");
    return 1;
}

Mix_Music* loadMusic(const char* file) {//esse codigo ta la na main, ele é o codigo que carrega a musica também prestar atenção no q ele printa no terminal
    Mix_Music* music = Mix_LoadMUS(file);
    if (!music) {
        printf("Erro ao carregar música '%s': %s\n", file, Mix_GetError());
    } else {
        printf("Música carregada com sucesso.\n");
    }
    return music;
}

void playMusic(Mix_Music* music) {
    if (music == NULL) {
        printf("A música não foi carregada corretamente.\n");
        return;
    }
    
    Mix_VolumeMusic(0);//define o volume maximo da musica pelo q eu li se botar MIX_MAX_VOLUME fica no volume que braz mandou a musica
    if (Mix_PlayMusic(music, -1) == -1) {
        printf("Erro ao tocar música: %s\n", Mix_GetError());
    } else {
        //se não aparecer isso no terminal é pq deu erro prestar atenção sempre nisso!
        printf("Música tocando...\n");
    }
}

void freeMusic(Mix_Music* music) {
    Mix_FreeMusic(music);
}

void closeAudio() {
    Mix_CloseAudio();
}
