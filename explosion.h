#pragma once

#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#define EXPLOSION_T_W 80
#define EXPLOSION_T_H 16
#define EXPLOSION_TASSEL_X 16
#define EXPLOSION_TASSEL_Y 16

typedef struct explosion_t{
    int frame;
    SDL_Texture *texture;
}explosion_t;

explosion_t *explosion_init(SDL_Renderer *renderer);

void border_limit_explosion(explosion_t *explosion, SDL_Rect *rect);

//calculates the speed and updates the position
void calc_speed_explosion(explosion_t *explosion);

void render_explosion(explosion_t *explosion);

void explosion_destroy(explosion_t *explosion);

#endif // _EXPLOSION_H