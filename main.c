#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "common.h"
#include "main_menu.h"
#include "game_stage.h"
#include "ship.h"

/*#define WINDOW_WIDTH 580
#define WINDOW_HEIGHT 720*/

int mix_wasInit = 0;

void init_all(){

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        handle_error("Error initializing SDL: %s",mix_wasInit);
        //return 0;
    }

    printf("SDL Initialized correctly!\n");

    if(IMG_Init(IMG_INIT_PNG) == 0){
        printf("Error img \n");
    }

    printf("SDL_IMG Initialized correctly!\n");

    mix_wasInit = Mix_Init(0);
    if(Mix_Init(0) == -1){
        handle_error("Error initializing Mix_int: %s",mix_wasInit);
        //return 0;
    }

    printf("Mix Initialized correctly!\n");

    if(TTF_Init() == -1){
        handle_error("Error initializing TTF_int: %s",mix_wasInit);
        //return 0;
    }

}

void close_init_all(){
    TTF_Quit();
    Mix_Quit();    
    SDL_Quit();
    IMG_Quit();
}

int main(int argc, char* argv[]){

    init_all();

    int ret;
    SDL_Window *window;
    SDL_Renderer *renderer;

    window = SDL_CreateWindow(
        "SpaceInvaders",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    
    if(window == NULL){
        handle_error("Could not create window: %s\n",mix_wasInit);
        return 1;
    }

    //create a renderer, for set up graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window,-1,render_flags);
    if(renderer == NULL){
        handle_error("Could not create renderer: %s\n",mix_wasInit);
        SDL_DestroyWindow(window);
        return 1;
    }

    // Select the color for drawing. It is set to black here.
    ret = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if(ret){
        handle_error("Could SetRenderDrawColor: %s\n",mix_wasInit);
        return 1;
    }

    // Clear the entire screen to our selected color.
    ret = SDL_RenderClear(renderer);
    if(ret){
        handle_error("Could RenderClear: %s\n",mix_wasInit);
        return 1;
    }

    // This will show the new, contents of the window.
    SDL_RenderPresent(renderer);

    //main loop
    int close_requested = 0;
    int level = 0;

    while(!close_requested){
        switch(level){
            case 0:
                main_menu(&close_requested,renderer,&level);
                break;
            case 1:
                game_stage(&close_requested,renderer,&level);
                break;
            }

    }

    //clean up
    close_init_all();
    printf("EXIT!\n");
    return 0;
}