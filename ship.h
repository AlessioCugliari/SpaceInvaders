#ifndef _SHIP_H
#define _SHIP_H

#define SHIP_SPEED 300

typedef struct ship_t{
    int lives;
    int speed_x;
    //keep track of the input given
    int left;
    int right;    
    float x_pos;
    float y_pos;
    SDL_Texture *texture;
}ship_t;

ship_t *ship_init(int lives, SDL_Renderer *renderer);

void border_limit_ship(ship_t *ship, SDL_Rect *rect);

//calculates the speed and updates the position
void calc_speed_ship(ship_t *ship);

void render_ship(ship_t *ship);

void ship_destroy(ship_t *ship);

#endif // _SHIP_H