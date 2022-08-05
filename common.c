#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

void handle_error(char *msg, int mix_wasInit){

    printf(msg,SDL_GetError());

    if(SDL_WasInit(SDL_INIT_EVERYTHING)){
        SDL_Quit();
    }
    if(mix_wasInit){
        Mix_Quit();
    }
    if(TTF_WasInit()){
        TTF_Quit(); 
    }
    //exit(EXIT_FAILURE);
}

TTF_Font *text_init(char *path){

    if(!TTF_WasInit()){
        printf("TTF was not init\n");
        exit(EXIT_FAILURE);        
    }

    TTF_Font *font = TTF_OpenFont(path,32);

    if(!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
    }

    return font;
}

Mix_Chunk *mix_chunk_load(char *path){

    Mix_Chunk *sound = Mix_LoadWAV(path);
    
    if(!sound) printf("sound error: %s\n", Mix_GetError());
    
    return sound;
}

Mix_Music *music_load(char *path){

    Mix_Music *music = Mix_LoadMUS(path);
    
    if(!music) printf("music error: %s\n", Mix_GetError());
 
    return music;
}