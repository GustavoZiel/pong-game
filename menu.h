#ifndef _MENU_H_
#define _MENU_H_

#include <ncurses.h>

// // Sem mutex
// #define SPEED 1000
// #define TAM_X 100
// #define TAM_Y 12
// #define TAM_RAQUETE 10

// Configurações de dificuldade

// Difícil
#define SPEED 40000
#define TAM_X 140
#define TAM_Y 40
#define TAM_RAQUETE 5

// // Médio
// #define SPEED 70000
// #define TAM_X 140
// #define TAM_Y 40
// #define TAM_RAQUETE 10

// // Fácil
// #define SPEED 100000
// #define TAM_X 140
// #define TAM_Y 40
// #define TAM_RAQUETE 13

// Não altere nada daqui para baixo
#define PLAY 1
#define QUIT 2
#define HEIGHT_MENU 100
#define WIDTH_MENU 100

WINDOW *create_menu();
void highlight_option(WINDOW *menu_win, int highlight);
int print_menu(WINDOW *menu_win);

#endif