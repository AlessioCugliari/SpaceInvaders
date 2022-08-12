#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ship.h"
#include "common.h"

ship_t *ship_init(int lives, SDL_Renderer *renderer){

    ship_t *ship = (ship_t*)malloc(sizeof(ship_t));
    ship->lives = lives;
    ship->speed_x = SHIP_SPEED;
    ship->left  = 0;
    ship->right = 0;
    ship->x_pos = WINDOW_WIDTH / 2;
    ship->y_pos = WINDOW_HEIGHT - 80;

    SDL_Surface *surf = IMG_Load("texture/ship5.png");
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

    ship->texture = texture;
    
    return ship;

}

void border_limit_ship(ship_t *ship, SDL_Rect *rect){

    if(ship->x_pos <= 0) ship->x_pos = 0;
    if(ship->y_pos <= 0) ship->y_pos = 0;
    if(ship->x_pos >=  WINDOW_WIDTH - rect->w) ship->x_pos = WINDOW_WIDTH - rect->w;
    if(ship->y_pos >=  WINDOW_HEIGHT - rect->h) ship->y_pos = WINDOW_HEIGHT - rect->h;
    //printf("ship x pos ship fiel %d\n",ship_rect_dest->x);
}

void calc_speed_ship(ship_t *ship){

    ship->speed_x = 0;  //without this it continues to move

    if(ship->left && !ship->right) ship->speed_x = -SHIP_SPEED;
    if(ship->right && !ship->left) ship->speed_x = SHIP_SPEED;
    
    ship->x_pos += ship->speed_x / FPS;
    //ship->y_pos += ship->y_vel / FPS;
}

void render_ship(ship_t *ship);

void ship_destroy(ship_t *ship){

    SDL_DestroyTexture(ship->texture);
    free(ship);
    printf("ship free ok\n");

}