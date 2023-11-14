#ifndef _GAME_H_
#define _GAME_H_

#include "menu.h"

typedef struct pair_t {
    int x, y;
} PAIR;

typedef struct ball_t {
    char icon;
    PAIR pos;
} BALL;

typedef struct raquete_t {
    char icon;
    int tam;
    PAIR pos;
} RAQUETE;

typedef struct game_win_t {
    WINDOW *win_obj;
    int height, width;
} WIN;

typedef struct {
    WIN *win;
    RAQUETE *raquete;
    int keys[2];
} JOGADOR;

typedef struct {
    WIN *win;
    JOGADOR *jogadores[2];
    BALL *ball;
} SETUP;

WIN *create_game_win();

BALL *create_ball(int icon, int posy, int posx);
void draw_ball(WIN *game_win, BALL *ball);
void erase_ball(WIN *game_win, BALL *ball);
void *move_ball_wrapper(void *args);

RAQUETE *create_raquete(int icon, int tam, int posy, int posx);
void draw_raquete(WIN *win, RAQUETE *raq, int move);
void erase_raquete(WIN *win, RAQUETE *raq);
void *move_raquete_wrapper(void *args);

JOGADOR *create_jogador(WIN *win, RAQUETE *raq, int key1, int key2);
SETUP *create_setup();
void free_setup(SETUP *setup);

void play_game(SETUP *setup);
int create_game();

#endif