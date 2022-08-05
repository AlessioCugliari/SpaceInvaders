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
#include "explosion.h"

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

static int hit_laser(SDL_Rect **arr, SDL_Rect *src, SDL_Rect *dest, SDL_Rect *laser, SDL_Renderer *renderer, SDL_Texture *texture, Mix_Chunk *sound){
    
    for(int i = 0; i < ENEMY_ROWS; i++){
        for(int j = 0; j < ENEMY_COLS; j++){
            if(SDL_HasIntersection(&arr[i][j],laser)){
                
                Mix_PlayChannel(-1,sound,0);
                dest->x = arr[i][j].x;
                dest->y = arr[i][j].y;
                //reduce size to not display the enemy ship
                arr[i][j].w = 0;
                arr[i][j].h = 0;
                laser->x = 800;
                laser->y = 800;
            
                return 1;
            }        
        }
    }  
    return 0;
}

static int game_over(int n_enemy, int lives, int y_pos){
    if(lives == 0 || y_pos > 500){
        printf("Defeat\n");
        return 1;
    }
    if(n_enemy == 0){
        printf("Vicotiy\n");
        return 2;
    }

    return 0;
}

void game_stage(int *close_requested, SDL_Renderer *renderer, int *level){

    printf("GAME STAGE\n");

    int n_enemy = ENEMY_ROWS*ENEMY_COLS;
    int hit = 0, step = 0, frame = 0;
    int game_status = 0;
    //int e_hit = 0, e_step = 0, e_hit_frame = 0;

    TTF_Font *font = text_init("font/Games.ttf");

    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024) == -1){
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        *close_requested = 1;
        return;
    }

    Mix_Chunk *laser_sound = mix_chunk_load("audio/Laser-weapon.wav");
    Mix_Chunk *explosion_sound = mix_chunk_load("audio/Explosion.wav");

    Mix_Music *music = music_load("audio/battle.wav");

    ship_t *ship = ship_init(3,renderer); 

    SDL_Rect ship_rect_dest;
    set_rect(&ship_rect_dest, WINDOW_WIDTH / 2, WINDOW_HEIGHT - 80, SHIP_TASSEL_X * 3, SHIP_TASSEL_Y * 3);

    int k = 2;
    SDL_Rect ship_rect_src;
    ship_rect_src.x = SHIP_TASSEL_X * k; 
    ship_rect_src.y = SHIP_TASSEL_Y;
    ship_rect_src.w = SHIP_TASSEL_X;
    ship_rect_src.h = SHIP_TASSEL_Y;

    explosion_t *ex = explosion_init(renderer);
    
    SDL_Rect ex_rect_dest;
    set_rect(&ex_rect_dest, 800, 800, EXPLOSION_TASSEL_X * 3, EXPLOSION_TASSEL_X * 3);
    
    SDL_Rect ex_rect_src;
    set_rect(&ex_rect_src, 0, 0, EXPLOSION_TASSEL_X, EXPLOSION_TASSEL_Y);
    
    //LASER
    laser_t *laser = laser_init(renderer);
    
    SDL_Rect laser_rect_dest;
    set_rect(&laser_rect_dest, 800, 800, LASER_TASSEL_X * 3, LASER_TASSEL_Y * 3);

    SDL_Rect laser_rect_src;
    set_rect(&laser_rect_src, LASER_TASSEL_X, LASER_TASSEL_Y, LASER_TASSEL_X, LASER_TASSEL_Y);

    laser_t *enemy_laser = laser_init_no_tex(renderer);

    SDL_Rect laser_enemy_rect_dest;
    set_rect(&laser_enemy_rect_dest, 500, 500, LASER_TASSEL_X * 3, LASER_TASSEL_Y * 3);

    SDL_Rect laser_enemy_rect_src;
    set_rect(&laser_enemy_rect_src, LASER_TASSEL_X, 0, LASER_TASSEL_X, LASER_TASSEL_Y);

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
    set_rect(&enemy_rect_src, ENEMY_TASSEL_X, 0, ENEMY_TASSEL_X, ENEMY_TASSEL_Y);
    
    /*int r,g,b;
    SDL_GetTextureColorMod(ship->texture,&r,&g,&b);*/

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
                            //GO TO THE MAIN MENU WITH ESC KEY
                            *level = 0;
                            break;
                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            k = 4;
                            ship->right = 1;
                            ship_rect_src.x = SHIP_TASSEL_X * k;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            k = 0;
                            ship->left = 1;
                            ship_rect_src.x = SHIP_TASSEL_X * k;
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
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            k = 2;
                            ship->left = 0;
                            ship_rect_src.x = SHIP_TASSEL_X * k;
                            break;
                    }
                    break;
            }
        }

        //music 
        if(!Mix_PlayingMusic()){
            Mix_PlayMusic(music, -1);
        }
 
        //SHIP MOV
        border_limit_ship(ship,&ship_rect_dest);
        calc_speed_ship(ship);
        ship_rect_dest.x = (int)ship->x_pos;

        //LASER MOV
        border_limit_laser(laser,&laser_rect_dest);
        calc_speed_laser(laser,LASER_SPEED);
        laser_rect_dest.y = (int)laser->y_pos;

        //ENEMY LASER MOV
        if(!enemy_laser->fired){    
            if(n_enemy) enemy_attack(enemy_laser, arr_enemy_rect, &laser_enemy_rect_dest,n_enemy);
            enemy_laser->y_pos = (float)laser_enemy_rect_dest.y;
        }
       
        //border_limit_laser(enemy_laser,&laser_enemy_rect_dest);
        calc_speed_laser(enemy_laser,-LASER_SPEED);
        laser_enemy_rect_dest.y = (int)enemy_laser->y_pos;
        if(enemy_laser->y_pos >= WINDOW_HEIGHT){
            //enemy_laser->y_pos = 0.0;
            enemy_laser->fired = 0;
            enemy_laser->down = 0;
            enemy_laser->speed_y = 0;
            laser_enemy_rect_dest.x = 700;
        }
        //enemy laser hit
        if(SDL_HasIntersection(&ship_rect_dest,&laser_enemy_rect_dest)){
            ship->lives--;
            enemy_laser->fired = 0;
            enemy_laser->down = 0;
            enemy_laser->speed_y = 0;
            laser_enemy_rect_dest.x = 700;
            /*e_hit = 1;
            SDL_SetTextureColorMod(ship->texture,250,0,0);
            SDL_SetTextureColorMod(ship->texture,r,g,b);*/
        }

        //game over check
        game_status = game_over(n_enemy,ship->lives,500);
        if(game_status){
            *close_requested = 1;
            break;
        }

        update_position_matrix(arr_enemy_rect,&curr_x,&matrix_w_pos,ENEMY_ROWS,ENEMY_COLS);
        
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer,laser->texture,&laser_rect_src,&laser_rect_dest);
        SDL_RenderCopy(renderer,ship->texture,&ship_rect_src,&ship_rect_dest);
        SDL_RenderCopy(renderer,laser->texture,&laser_enemy_rect_src,&laser_enemy_rect_dest);
        
        render_arr_enemy(arr_enemy_rect, &enemy_rect_src, enemy->texture, renderer);

        SDL_RenderCopy(renderer,ex->texture,&ex_rect_src,&ex_rect_dest);
        
        if(hit_laser(arr_enemy_rect, &ex_rect_src, &ex_rect_dest, &laser_rect_dest, renderer, ex->texture, explosion_sound)){
            laser->fired = 0;
            laser->down = 0;
            n_enemy--;
            printf("%d\n", n_enemy);
            hit = 1;
            
        }
        //To render the explosion
        if(hit){
            if(!(frame % EXPLOSION_TASSEL_X)){
                step++;
            }
            frame += 4; 
            ex_rect_src.x = EXPLOSION_TASSEL_X*step;
            if(frame >= EXPLOSION_T_W){
                frame = 0;
                step = 0;
                hit = 0;
            }
        } 
        SDL_RenderCopy(renderer,ex->texture,&ex_rect_src,&ex_rect_dest);  
        
        SDL_RenderPresent(renderer);
    }

    if(game_status){
        
        int x = (WINDOW_WIDTH - 500) >> 1;
        SDL_Rect rect;
        set_rect(&rect,x,250,500,200);
        
        SDL_Rect second_line_rect;
        set_rect(&second_line_rect,x,450,500,50);

        SDL_Color white = {255,255,255};
        SDL_Color grey  = {182,182,182};

        SDL_Surface *surf;
        SDL_Texture *texture;
        SDL_Texture *second_line_texture;

        *close_requested = 0;

        int mouse_x, mouse_y;
        int buttons = 0;

        if(game_status == 1){
            //DEFEAT
            surf = TTF_RenderText_Solid(font,"DEFEAT", white);
            texture = SDL_CreateTextureFromSurface(renderer,surf);

            surf = TTF_RenderText_Solid(font,"click anywhere to return to the main menu",grey);
            second_line_texture = SDL_CreateTextureFromSurface(renderer,surf);
            SDL_FreeSurface(surf);
        }
        else{
            //VICOTRY
            surf = TTF_RenderText_Solid(font,"VICTORY", white);
            texture = SDL_CreateTextureFromSurface(renderer,surf);

            surf = TTF_RenderText_Solid(font,"click anywhere to return to the main menu",grey);
            second_line_texture = SDL_CreateTextureFromSurface(renderer,surf);
            SDL_FreeSurface(surf);
        }

        while(!*close_requested && *level){
            while(SDL_PollEvent(&event)){
                switch(event.type){
                    case SDL_QUIT:
                        *close_requested = 1;
                        break;
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.scancode){
                            case SDL_SCANCODE_ESCAPE:
                                //GO TO THE MAIN MENU
                                *level = 0;
                                break;
                        }
                }
            }
            //get cursor position relative to window
            buttons = SDL_GetMouseState(&mouse_x,&mouse_y);
            //exit with left click
            if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
                *level = 0;
            }
            SDL_RenderCopy(renderer,texture,NULL,&rect);
            SDL_RenderCopy(renderer,second_line_texture,NULL,&second_line_rect);
            SDL_RenderPresent(renderer);
        }

        TTF_CloseFont(font);
        SDL_DestroyTexture(texture);
    }

    ship_destroy(ship);
    laser_destroy(laser);
    free(enemy_laser);
    Mix_FreeChunk(laser_sound);
    Mix_FreeChunk(explosion_sound);
    Mix_FreeMusic(music);
    free_arr_enemy(arr_enemy_rect);
    enemy_destroy(enemy);
    explosion_destroy(ex);

    printf("Exit Game Stage\n");
}