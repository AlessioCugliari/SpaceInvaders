#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "ship.h"
#include "common.h"

#define SHIP_T_W 80
#define SHIP_T_H 48
#define SHIP_TASSEL_X 16
#define SHIP_TASSEL_Y 24

void game_stage(int *close_requested, SDL_Renderer *renderer, int *level){

    printf("GAME STAGE\n");

    ship_t *ship = ship_init(3,renderer); 

    SDL_Surface *surf = IMG_Load("texture/ship.png");
    if(!surf){
        printf("Can not load surface for ship %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surf);

    //free surf we don't need anymore
    SDL_FreeSurface(surf);

    if(!texture){
        printf("Can not load texture for ship %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Rect ship_rect_dest;
    ship_rect_dest.x = WINDOW_WIDTH / 2; 
    ship_rect_dest.y = WINDOW_HEIGHT - 80;
    ship_rect_dest.w = SHIP_TASSEL_X * 3;
    ship_rect_dest.h = SHIP_TASSEL_Y * 3;

    int k = 2;
    SDL_Rect ship_rect_src;
    ship_rect_src.x = SHIP_TASSEL_X * k; 
    ship_rect_src.y = SHIP_TASSEL_Y;
    ship_rect_src.w = SHIP_TASSEL_X;
    ship_rect_src.h = SHIP_TASSEL_Y;

    SDL_Event event;
    while(!*close_requested && *level){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    *close_requested = 1;
                    ship_destroy(ship);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode){
                        case SDL_SCANCODE_ESCAPE:
                            *level = 0;
                            ship_destroy(ship);
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            k = 4;
                            ship->right = 1;
                            ship_rect_src.x = SHIP_TASSEL_X * k;
                            //printf("press D %d\n",ship_rect_src.x);
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            k = 0;
                            ship->left = 1;
                            ship_rect_src.x = SHIP_TASSEL_X * k;
                            //printf("press A %d\n",ship_rect_src.x);
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.scancode){
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            k = 2;
                            ship->right = 0;
                            ship_rect_src.x = SHIP_TASSEL_X * k;
                            //printf("release d %d\n",ship_rect_src.x);
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            k = 2;
                            ship->left = 0;
                            ship_rect_src.x = SHIP_TASSEL_X * k;
                            //printf("release a %d\n",ship_rect_src.x);
                            break;
                    }
                    break;
            }
        }
 
        border_limit_ship(ship,&ship_rect_dest);
        calc_speed_ship(ship);

        ship_rect_dest.x = (int)ship->x_pos;

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer,ship->texture,&ship_rect_src,&ship_rect_dest);

        SDL_RenderPresent(renderer);
    }

    printf("Exit Game Stage\n");
}