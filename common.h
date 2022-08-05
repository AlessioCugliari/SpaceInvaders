#ifndef _COMMON_H
#define _COMMON_H

#define WINDOW_WIDTH 580
#define WINDOW_HEIGHT 720
#define FPS 60

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void handle_error(char *msg,int mix_wasInit);

TTF_Font *text_init(char *path);

Mix_Chunk *mix_chunk_load(char *path);

Mix_Music *music_load(char *path);

#endif // _COMMON_H