#pragma once

#ifndef _ENEMY_H
#define _ENEMY_H

#define ENEMY_SPEED 20
#define ENEMY_W 64
#define ENEMY_H 16
#define ENEMY_TASSEL_X 32
#define ENEMY_TASSEL_Y 16

typedef struct enemy_t{
    int speed_x;
    //keep track of the input given
    int left;
    int right;    
    float x_pos;
    float y_pos;
    SDL_Texture *texture;
}enemy_t;

enemy_t *enemy_init(SDL_Renderer *renderer);

void border_limit_enemy(enemy_t *enemy, SDL_Rect *rect);

//calculates the speed and updates the position
void calc_speed_enemy(enemy_t *enemy, int x, int w);

//for the enemy matrix
void update_position_matrix(SDL_Rect **arr,int *x, int *w ,int r, int c);

void enemy_destroy(enemy_t *enemy);

#endif // _ENEMY_H