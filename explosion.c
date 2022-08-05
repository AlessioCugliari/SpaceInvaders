#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "explosion.h"
#include "common.h"

explosion_t *explosion_init(SDL_Renderer *renderer){

    explosion_t *explosion = (explosion_t*)malloc(sizeof(explosion_t));

    explosion->frame = 0;

    SDL_Surface *surf = IMG_Load("texture/explosion.png");
    if(!surf){
        printf("Can not load surface for explosion %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surf);

    //free surf we don't need anymore
    SDL_FreeSurface(surf);

    if(!texture){
        printf("Can not load texture for explosion %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    explosion->texture = texture;

    return explosion;

}

void explosion_destroy(explosion_t *explosion){

    SDL_DestroyTexture(explosion->texture);
    free(explosion);
    printf("explosion free ok\n");

}