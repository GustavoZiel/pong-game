#include "logic.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "menu.h"

pthread_mutex_t mutex;
pthread_cond_t cond;
int game_on;
int player_won;
int key;
int ballDirX, ballDirY;

int flag_mutex = 1;

void atualizar_tela(WINDOW* win, int flag_mutex){
    if(flag_mutex == 1){
        pthread_mutex_lock(&mutex);
    }
    wrefresh(win);
    pthread_mutex_unlock(&mutex);
}

WIN *create_game_win() {
    WIN *game_win;
    game_win = malloc(sizeof(WIN));

    int height = TAM_Y;
    int width = TAM_X;

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW *win;
    win = newwin(height, width, starty, startx);
    keypad(win, TRUE);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');

    game_win->win_obj = win;
    game_win->height = height;
    game_win->width = width;

    return game_win;
}

BALL *create_ball(int icon, int posy, int posx) {
    BALL *ball;
    ball = malloc(sizeof(BALL));

    ball->icon = icon;
    ball->pos.x = posx;
    ball->pos.y = posy;

    return ball;
}
int ball_collision(WIN *game_win, BALL *ball, RAQUETE *raquetes[2]) {
    // Movendo a bola
    ball->pos.x += ballDirX;
    ball->pos.y += ballDirY;

    // Cálculo das colisões
    if (ball->pos.x == 3 && (ball->pos.y >= raquetes[0]->pos.y && ball->pos.y < raquetes[0]->pos.y + raquetes[0]->tam)) {
        // Colidiu com a raquete do player 1
        ballDirX *= -1;
    }

    if (ball->pos.x == game_win->width - 4 && (ball->pos.y >= raquetes[1]->pos.y && ball->pos.y < raquetes[1]->pos.y + raquetes[1]->tam)) {
        // Colidiu com a raquete do player 2
        ballDirX *= -1;
    }

    if (ball->pos.y == game_win->height - 2 || ball->pos.y == 1) {
        // Colidiu com a parede superior ou inferior da arena 
        ballDirY *= -1;
    }

    if (ball->pos.x == game_win->width - 1) {
        // Colidiu com a parede esquerda da arena, vitória do player 1
        game_on = 0;
        player_won = 1;
    }

    if (ball->pos.x == 0) {
        // Colidiu com a parede esquerda da arena, vitória do player 2 
        game_on = 0;
        player_won = 2;
    }
}
void draw_ball(WIN *game_win, BALL *ball) {
    mvwaddch(game_win->win_obj, ball->pos.y, ball->pos.x, ball->icon);
    atualizar_tela(game_win->win_obj,flag_mutex);
}
void erase_ball(WIN *game_win, BALL *ball) {
    mvwaddch(game_win->win_obj, ball->pos.y, ball->pos.x, ' ');
    atualizar_tela(game_win->win_obj,flag_mutex);
}
void *move_ball_wrapper(void *args) {
    SETUP *setup = (SETUP *)args;

    RAQUETE *raquetes[2];
    raquetes[0] = setup->jogadores[0]->raquete;
    raquetes[1] = setup->jogadores[1]->raquete;

    while (game_on) {
        erase_ball(setup->win, setup->ball);
        
        atualizar_tela(setup->win->win_obj,flag_mutex);
        
        ball_collision(setup->win, setup->ball, raquetes);
        draw_ball(setup->win, setup->ball);
        usleep(SPEED);
    }
}

RAQUETE *create_raquete(int icon, int tam, int posy, int posx) {
    RAQUETE *raq;
    raq = malloc(sizeof(RAQUETE));

    raq->icon = icon;
    raq->tam = tam;

    raq->pos.y = posy;
    raq->pos.x = posx;

    return raq;
}
void draw_raquete(WIN *win, RAQUETE *raq, int move) {
    int new_pos = raq->pos.y + move;
    if (new_pos != 0 && (new_pos + raq->tam) != win->height) {
        raq->pos.y = new_pos;
    }
    for (int i = 0; i < raq->tam; i++) {
        mvwaddch(win->win_obj, raq->pos.y + i, raq->pos.x, raq->icon);
    }
    atualizar_tela(win->win_obj,flag_mutex);
}
void erase_raquete(WIN *win, RAQUETE *raq) {
    for (int i = 0; i < raq->tam; i++) {
        mvwaddch(win->win_obj, raq->pos.y + i, raq->pos.x, ' ');
    }
    pthread_mutex_lock(&mutex);
    wrefresh(win->win_obj);
    pthread_mutex_unlock(&mutex);
}
void *move_raquete_wrapper(void *args) {
    JOGADOR *jogador = (JOGADOR *)args;
    while (game_on) {
        if (key == jogador->keys[0]) {
            erase_raquete(jogador->win, jogador->raquete);
            draw_raquete(jogador->win, jogador->raquete, -1);
            key = -1;
        } else if (key == jogador->keys[1]) {
            erase_raquete(jogador->win, jogador->raquete);
            draw_raquete(jogador->win, jogador->raquete, +1);
            key = -1;
        }
    }
}

JOGADOR *create_jogador(WIN *win, RAQUETE *raq, int key1, int key2) {
    JOGADOR *jogador;
    jogador = malloc(sizeof(jogador));

    jogador->win = win;
    jogador->raquete = raq;
    jogador->keys[0] = key1;
    jogador->keys[1] = key2;

    return jogador;
}

SETUP *create_setup() {
    SETUP *setup;
    setup = malloc(sizeof(SETUP));

    setup->win = create_game_win();
    RAQUETE *raquete1, *raquete2;
    raquete1 = create_raquete('|', TAM_RAQUETE, 1, 2);
    raquete2 = create_raquete('|', TAM_RAQUETE, 1, setup->win->width - 3);

    JOGADOR *jogador1, *jogador2;
    int keys[] = {'w', 's', KEY_UP, KEY_DOWN};
    jogador1 = create_jogador(setup->win, raquete1, keys[0], keys[1]);
    jogador2 = create_jogador(setup->win, raquete2, keys[2], keys[3]);

    setup->jogadores[0] = jogador1;
    setup->jogadores[1] = jogador2;

    setup->ball = create_ball('o', setup->win->height / 2, setup->win->width / 2);

    return setup;
}
void free_setup(SETUP *setup) {
    free(setup->win);

    free(setup->jogadores[0]->raquete);
    free(setup->jogadores[1]->raquete);

    free(setup->jogadores[0]);
    free(setup->jogadores[1]);

    free(setup->ball);
    free(setup);
}

void play_game(SETUP *setup) {
    // Desenhando inicialmente a arena
    draw_raquete(setup->win, setup->jogadores[0]->raquete, 0);
    draw_raquete(setup->win, setup->jogadores[1]->raquete, 0);
    draw_ball(setup->win, setup->ball);

    // Solicitando ao usuário que presse ENTER para que o jogo comece
    char *start_message = "Press ENTER to start the game!";
    char *erase_message = "                              ";
    int tam_message = strlen(start_message);
    mvwprintw(setup->win->win_obj, setup->ball->pos.y - 3, setup->ball->pos.x - tam_message / 2, start_message);
    atualizar_tela(setup->win->win_obj,flag_mutex);
    int key_start;
    while ((key_start = getch()) != 10);
    mvwprintw(setup->win->win_obj, setup->ball->pos.y - 3, setup->ball->pos.x - tam_message / 2, erase_message);
    atualizar_tela(setup->win->win_obj,flag_mutex);

    // Determinando aleatóriamenet direção inicial da bola
    srand(time(0));
    ballDirX = rand() % 2 == 0 ? 1 : -1;
    ballDirY = rand() % 2 == 0 ? 1 : -1;

    // Iniciando threads
    pthread_t p, q, r;
    pthread_create(&p, NULL, move_raquete_wrapper, (void *)(setup->jogadores[0]));
    pthread_create(&q, NULL, move_raquete_wrapper, (void *)(setup->jogadores[1]));
    pthread_create(&r, NULL, move_ball_wrapper, (void *)(setup));

    while (game_on != 0) {
        key = getch();
        if (key == 'q') {
            game_on = 0;
        }
    }
}
int end_game(SETUP *setup) {
    char *game_overM1 = "==== GAME OVER ====";
    char *game_overM2 = "===================";
    int game_overM1_size = strlen(game_overM1);

    char *play_again = "Press R to play again or Q to quit";
    int play_again_size = strlen(play_again);

    char *player_wowM;
    int player_wowM_size;

    switch (player_won) {
        case 1:
            player_wowM = "Player 1 Won";
            player_wowM_size = strlen(player_wowM);
            break;

        case 2:
            player_wowM = "Player 2 Won";
            player_wowM_size = strlen(player_wowM);
            break;

        default:
            player_wowM = "Game aborted";
            player_wowM_size = strlen(player_wowM);
            break;
    }

    // Printing messages
    mvwprintw(setup->win->win_obj, (setup->win->height / 2) - 5, setup->win->width / 2 - game_overM1_size / 2, game_overM1);
    mvwprintw(setup->win->win_obj, (setup->win->height / 2) - 4, setup->win->width / 2 - player_wowM_size / 2, player_wowM);
    mvwprintw(setup->win->win_obj, (setup->win->height / 2) - 3, setup->win->width / 2 - game_overM1_size / 2, game_overM2);
    mvwprintw(setup->win->win_obj, (setup->win->height / 2) + 1, setup->win->width / 2 - play_again_size / 2, play_again);

    atualizar_tela(setup->win->win_obj,flag_mutex);

    int key_continue;
    do {
        key_continue = getch();
    } while (key_continue != 'r' && key_continue != 'q');
    clear();
    delwin(setup->win->win_obj);

    if (key_continue == 'r')
        return PLAY;
    else
        return QUIT;
}

int create_game() {
    // Inicializando variáveis globais
    game_on = 1;
    player_won = -1;
    key = -1;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    SETUP *setup;
    setup = create_setup();
    play_game(setup);
    int choice = end_game(setup);
    free_setup(setup);
    return choice;
}