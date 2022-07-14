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

