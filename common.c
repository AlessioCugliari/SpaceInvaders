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

void set_rect(SDL_Rect *rect, int x, int y, int w, int h){

    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
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

SDL_Texture *texture_load(char *path, SDL_Renderer *renderer){

    SDL_Surface *surf = IMG_Load(path);
    if(!surf){
        printf("Can not load surface %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surf);

    //free surf we don't need anymore
    SDL_FreeSurface(surf);

    if(!texture){
        printf("Can not load texture %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    printf("texture load %s ok\n",path);

    return texture;
}

int animation_update(int *frame, int *step, SDL_Rect *src, int tassel_x, int texture_witdh){

    int temp = *step;
    temp++;

    if(!(*frame % tassel_x)){
        *step = temp;
    }
    *frame += 4; 
    src->x = tassel_x*(*step);
    if(*frame >= texture_witdh){
        *frame = 0;     //reset frame to 0 for the next animetion
        *step = 0;      //reset step to 0 for the next animetion
        return 0;       //set the animation variable to 0
    }
    return 1;
}