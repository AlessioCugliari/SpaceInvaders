#ifndef _GAME_STAGE_H
#define _GAME_STAGE_H

#define ENEMY_ROWS 6
#define ENEMY_COLS 5

#define ASTE_ROWS 2
#define ASTE_COLS 9

void game_stage(int *close_requested, SDL_Renderer *renderer, int *level);

#endif // _GAME_STAGE_H