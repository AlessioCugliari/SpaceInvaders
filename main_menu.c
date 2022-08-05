#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "common.h"

void main_menu(int *close_requested, SDL_Renderer *renderer, int *level){

    if(!TTF_WasInit()){
        printf("TTF was not init\n");
        exit(EXIT_FAILURE);        
    }

    /*if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024) == -1){
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        *close_requested = 1;
        return;
    }

    Mix_Chunk *click_sound = mix_chunk_load("audio/click.wav");*/

    SDL_Color white = {255,255,255};

    TTF_Font *font = TTF_OpenFont("font/Games.ttf",32);

    if(!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
    }

    SDL_Surface *game_title_surf = TTF_RenderText_Solid(font,"SPACE INVADERS", white);

    SDL_Texture *game_title_texture = SDL_CreateTextureFromSurface(renderer,game_title_surf);
    SDL_FreeSurface(game_title_surf);

    SDL_Rect title_rect;

    title_rect.w = 500;
    title_rect.h = 200;
    title_rect.x = (WINDOW_WIDTH - title_rect.w) >> 1;
    title_rect.y = 50;

    SDL_Surface *new_game_surf = TTF_RenderText_Solid(font,"New Game", white);

    SDL_Texture *new_game_texture = SDL_CreateTextureFromSurface(renderer,new_game_surf);
    SDL_FreeSurface(new_game_surf);

    SDL_Rect newgame_rect;

    newgame_rect.w = 200;
    newgame_rect.h = 50;
    newgame_rect.x = (WINDOW_WIDTH - newgame_rect.w) >> 1;
    newgame_rect.y = 400;

    SDL_Surface *exit_surf = TTF_RenderText_Solid(font,"Exit",white);

    SDL_Texture *exit_texture = SDL_CreateTextureFromSurface(renderer,exit_surf);
    SDL_FreeSurface(exit_surf);

    SDL_Rect exit_rect;

    exit_rect.w = 100;
    exit_rect.h = 50;
    exit_rect.x = (WINDOW_WIDTH - exit_rect.w) >> 1;
    exit_rect.y = 500;

    SDL_Rect mouse_rect;
    
    mouse_rect.w = 1;
    mouse_rect.h = 1;
    mouse_rect.x = 0;
    mouse_rect.y = 0;

    SDL_Event event;

    int mouse_int = 0;
    int mouse_int_e = 0;
    while(!*close_requested){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    *close_requested = 1;
                    break;
            }
        }

        //get cursor position relative to window
        int mouse_x, mouse_y;
        int buttons = SDL_GetMouseState(&mouse_x,&mouse_y);

        mouse_rect.x = mouse_x;
        mouse_rect.y = mouse_y;

        //GO TO PLAY 
        if(SDL_HasIntersection(&mouse_rect,&newgame_rect)){
            SDL_SetTextureColorMod(new_game_texture,255,255,25);
            mouse_int = 1;
            if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
                printf("OK\n");
                //Mix_PlayChannel(-1,click_sound,0);
                *level = 1;
                break;
            }
        }
         if(mouse_int && !SDL_HasIntersection(&mouse_rect,&newgame_rect)){
            SDL_SetTextureColorMod(new_game_texture,255,255,255);
            mouse_int = 0;
        }

        //EXIT GAME
        if(SDL_HasIntersection(&mouse_rect,&exit_rect)){
            SDL_SetTextureColorMod(exit_texture,255,255,25);
            mouse_int_e = 1;
            if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
                //Mix_PlayChannel(-1,click_sound,0);
                *close_requested = 1;
                *level = 0;
            }
        }

        if(mouse_int_e && !SDL_HasIntersection(&mouse_rect,&exit_rect)){
            SDL_SetTextureColorMod(exit_texture,255,255,255);
            mouse_int_e = 0;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer,game_title_texture,NULL,&title_rect);
        SDL_RenderCopy(renderer,new_game_texture,NULL,&newgame_rect);
        SDL_RenderCopy(renderer,exit_texture,NULL,&exit_rect);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(game_title_texture);
    SDL_DestroyTexture(new_game_texture);
    SDL_DestroyTexture(exit_texture);
    //Mix_FreeChunk(click_sound);
    TTF_CloseFont(font);
    printf("Main_menu_exit\n");
}