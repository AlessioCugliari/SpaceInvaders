#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "ship.h"
#include "common.h"
#include "laser.h"
#include "enemy.h"
#include "game_stage.h"

static void free_arr_enemy(SDL_Rect **arr){

    for(int i = 0; i < ENEMY_ROWS; i++){
        free(arr[i]);
    }
    free(arr);
    printf("free arr ene opk\n");
}

static void render_arr_enemy(SDL_Rect **arr, SDL_Rect *src, SDL_Texture *texture, SDL_Renderer *renderer){

    for(int i = 0; i < ENEMY_ROWS; i++){
        for(int j = 0; j < ENEMY_COLS; j++){
            SDL_RenderCopy(renderer,texture,src,&arr[i][j]);
        }
    }
}

void game_stage(int *close_requested, SDL_Renderer *renderer, int *level){

    printf("GAME STAGE\n");

    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024) == -1){
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        *close_requested = 1;
        return;
    }

    Mix_Chunk *laser_sound = Mix_LoadWAV("audio/Laser-weapon.wav");
    if(!laser_sound){
        printf("laser_sound error: %s\n", Mix_GetError());
    }

    ship_t *ship = ship_init(3,renderer); 

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

    laser_t *laser = laser_init(renderer);
    
    SDL_Rect laser_rect_dest;
    laser_rect_dest.x = 800;
    laser_rect_dest.y = 800;
    laser_rect_dest.w = LASER_TASSEL_X * 3;
    laser_rect_dest.h = LASER_TASSEL_Y * 3;

    SDL_Rect laser_rect_src;
    laser_rect_src.x = LASER_TASSEL_X;
    laser_rect_src.y = LASER_TASSEL_Y;
    laser_rect_src.w = LASER_TASSEL_X;
    laser_rect_src.h = LASER_TASSEL_Y;

    enemy_t *enemy = enemy_init(renderer);
    
    SDL_Rect **arr_enemy_rect = malloc(ENEMY_ROWS*sizeof(SDL_Rect));
    
    for(int i = 0; i < ENEMY_ROWS; i++){
        arr_enemy_rect[i] = malloc(ENEMY_COLS*sizeof(SDL_Rect));
    }

    int e_t_x = ENEMY_TASSEL_X * 1.5; 
    int e_t_y = ENEMY_TASSEL_Y * 1.5;
    int curr_x = 0;
    for(int i = 0; i < ENEMY_ROWS; i++){
        curr_x += e_t_x;
        for(int j = 0; j < ENEMY_COLS; j++){
            arr_enemy_rect[i][j].x = curr_x;
            arr_enemy_rect[i][j].y = (j * e_t_y) + LASER_TASSEL_Y;
            arr_enemy_rect[i][j].w = e_t_x;
            arr_enemy_rect[i][j].h = e_t_y;
            
        }
        curr_x += 30;
    }


    int matrix_w_pos = arr_enemy_rect[ENEMY_ROWS-1][ENEMY_COLS-1].x + e_t_x;     
    curr_x = e_t_x;

    SDL_Rect enemy_rect_src;
    enemy_rect_src.x = ENEMY_TASSEL_X;
    enemy_rect_src.y = 0;
    enemy_rect_src.w = ENEMY_TASSEL_X;
    enemy_rect_src.h = ENEMY_TASSEL_Y;

    SDL_Event event;
    while(!*close_requested && *level){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    *close_requested = 1;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode){
                        case SDL_SCANCODE_ESCAPE:
                            *level = 0;
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
                        case SDL_SCANCODE_SPACE:
                            if(!laser->fired){
                                laser_rect_dest.x = ship_rect_dest.x + (SHIP_TASSEL_X / 3);
                                laser_rect_dest.y = ship_rect_dest.y;
                                laser->down = 1;
                                laser->y_pos = laser_rect_dest.y;
                                laser->fired = 1;
                                Mix_PlayChannel(-1,laser_sound,0);
                            } 
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

        border_limit_laser(laser,&laser_rect_dest);
        calc_speed_laser(laser);
        laser_rect_dest.y = (int)laser->y_pos;

        update_position_matrix(arr_enemy_rect,&curr_x,&matrix_w_pos,ENEMY_ROWS,ENEMY_COLS);
        
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer,laser->texture,&laser_rect_src,&laser_rect_dest);
        SDL_RenderCopy(renderer,ship->texture,&ship_rect_src,&ship_rect_dest);
        
        render_arr_enemy(arr_enemy_rect, &enemy_rect_src, enemy->texture, renderer);
        
        SDL_RenderPresent(renderer);
    }

    ship_destroy(ship);
    laser_destroy(laser);
    Mix_FreeChunk(laser_sound);
    free_arr_enemy(arr_enemy_rect);
    enemy_destroy(enemy);

    printf("Exit Game Stage\n");
}