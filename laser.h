#pragma once

#ifndef _LASER_H
#define _LASER_H

#define LASER_SPEED 300
#define LASER_W 32
#define LASER_H 32
#define LASER_TASSEL_X 16
#define LASER_TASSEL_Y 16

typedef struct laser_t{
    int speed_y;
    //keep track of the input given
    int fired;
    int down;    
    float x_pos;
    float y_pos;
    SDL_Texture *texture;
}laser_t;

laser_t *laser_init(SDL_Renderer *renderer);

laser_t *laser_init_no_tex(SDL_Renderer *renderer);

void enemy_attack(laser_t *laser, SDL_Rect **arr, SDL_Rect *rect,int enemy_left);

void border_limit_laser(laser_t *laser, SDL_Rect *rect);

//calculates the speed and updates the position
void calc_speed_laser(laser_t *laser, int speed);

//reset the coordinate after a collison
void laser_hit_reset(laser_t *laser);

void laser_destroy(laser_t *laser);

#endif // _LASER_H