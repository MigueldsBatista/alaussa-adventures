#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

int initAudio();

Mix_Music* loadMusic(const char* file);

void playMusic(Mix_Music* music);

void freeMusic(Mix_Music* music);

void closeAudio();

#endif
