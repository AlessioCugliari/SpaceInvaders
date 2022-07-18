#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "enemy.h"
#include "common.h"

enemy_t *enemy_init(SDL_Renderer *renderer){

    enemy_t *enemy = (enemy_t*)malloc(sizeof(enemy_t));

    enemy->speed_x = 1;
    enemy->right = 0;
    enemy->left = 0;
    enemy->x_pos = 0;
    enemy->y_pos = 0;

    SDL_Surface *surf = IMG_Load("texture/enemy-medium.png");
    if(!surf){
        printf("Can not load surface for enemy %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surf);

    //free surf we don't need anymore
    SDL_FreeSurface(surf);

    if(!texture){
        printf("Can not load texture for enemy %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    enemy->texture = texture;

    return enemy;
}

void border_limit_enemy(enemy_t *enemy, SDL_Rect *rect){

    /*if(enemy->y_pos <= ENEMY_TASSEL_Y >> 1){
        enemy->y_pos = -ENEMY_TASSEL_Y << 2;
        enemy->fired = 0;
        enemy->down = 0;
    }*/ 
    if(enemy->x_pos >=  WINDOW_WIDTH - rect->w){    
        if(enemy->x_pos ==  WINDOW_WIDTH - rect->w){
            enemy->left = 1;
            enemy->right = 0;
        }
        enemy->x_pos = WINDOW_WIDTH - rect->w;
    }   
    //if(enemy->y_pos >=  WINDOW_HEIGHT - rect->h) enemy->y_pos = WINDOW_HEIGHT - rect->h;
}

void calc_speed_enemy(enemy_t *enemy, int x, int w){

    //if(enemy->up && !enemy->down) enemy->speed_y = -enemy_SPEED;
    //if(enemy->down) enemy->speed_y = -ENEMY_SPEED;
    //if(enemy->x_pos >= WINDOW_WIDTH - w) enemy->x_pos = WINDOW_WIDTH - w;

    if(enemy->left && !enemy->right) enemy->speed_x = -ENEMY_SPEED;
    if(enemy->right && !enemy->left) enemy->speed_x = ENEMY_SPEED;

    enemy->x_pos += enemy->speed_x / FPS;
}

int step = 1;

void update_position_matrix(SDL_Rect **arr,int *x, int *w, int r, int c){

    *w += step;
    *x += step;
    if(*w > WINDOW_WIDTH || *x < 0){
        step = step * -1;

        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                arr[i][j].y += ENEMY_TASSEL_Y;
            }
        }
    }
    
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            arr[i][j].x += step;
        }
    }
    
}

void enemy_destroy(enemy_t *enemy){

    SDL_DestroyTexture(enemy->texture);
    free(enemy);
    printf("enemy free ok\n");
}