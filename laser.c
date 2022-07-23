#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "laser.h"
#include "game_stage.h"
#include "common.h"

laser_t *laser_init(SDL_Renderer *renderer){

    laser_t *laser = (laser_t*)malloc(sizeof(laser_t));

    laser->speed_y = 1;
    laser->fired = 0;
    laser->down = 0;
    laser->x_pos = 0;
    laser->y_pos = 0;

    SDL_Surface *surf = IMG_Load("texture/laser-bolts.png");
    if(!surf){
        printf("Can not load surface for laser %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,surf);

    //free surf we don't need anymore
    SDL_FreeSurface(surf);

    if(!texture){
        printf("Can not load texture for laser %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    laser->texture = texture;

    return laser;
}

laser_t *laser_init_no_tex(SDL_Renderer *renderer){

    laser_t *laser = (laser_t*)malloc(sizeof(laser_t));

    laser->speed_y = 1;
    laser->fired = 0;
    laser->down = 0;
    laser->x_pos = 0.0;
    laser->y_pos = 0.0;
    laser->texture = 0;

    return laser;

}

void enemy_attack(laser_t *laser, SDL_Rect **arr, SDL_Rect *rect,int enemy_left){
    
    srand(time(NULL));
    int n = 0;
    for(int i = 0; i < ENEMY_ROWS; i++){
        for(int j = 0; j < ENEMY_COLS; j++){
            if(arr[i][j].x != 800){
                n = rand() % enemy_left;
                if(n == 1){
                    //fire
                    rect->x = arr[i][j].x + (LASER_TASSEL_X / 3);
                    rect->y = arr[i][j].y;
                    laser->fired = 1;
                    laser->down = 1;
                    
                    /*printf("pos y arr[%d][%d] in ene mat %d\n",i,j,arr[i][j].y);
                    printf("pos x arr[%d][%d] in ene mat %d\n",i,j,arr[i][j].x);
                    printf("pos y REC[%d][%d] in ene mat %d\n",i,j,rect->y);
                    printf("pos x REC[%d][%d] in ene mat %d\n",i,j,rect->x);*/

                }
                
            }else{
                enemy_left--;
            }
        }
    } 

}

void border_limit_laser(laser_t *laser, SDL_Rect *rect){

    if(laser->y_pos <= LASER_TASSEL_Y >> 1 ){
        laser->y_pos = -LASER_TASSEL_Y << 2;
        laser->fired = 0;
        laser->down = 0;
    } 
    
    if(laser->y_pos >= WINDOW_HEIGHT - rect->h) laser->y_pos = WINDOW_HEIGHT - rect->h;
    
    
    //if(laser->y_pos <= 0) printf("_pos %f \n",laser->y_pos);
}

void calc_speed_laser(laser_t *laser, int speed){

    //if(laser->up && !laser->down) laser->speed_y = -LASER_SPEED;
    if(laser->down) laser->speed_y = -speed;
    
    laser->y_pos += laser->speed_y / FPS;
}

void laser_destroy(laser_t *laser){

    SDL_DestroyTexture(laser->texture);
    free(laser);
    printf("laser free ok\n");
}