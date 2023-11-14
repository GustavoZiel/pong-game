#include <stdio.h>
#include <stdlib.h>

#include "logic.h"
#include "menu.h"

int main() {
    WINDOW *menu_win;

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    refresh();

    menu_win = create_menu();
    int choice;
    choice = print_menu(menu_win);

    delwin(menu_win);
    refresh();
    while (choice != QUIT) {
        choice = create_game();
        clear();
        refresh();
    }

    endwin();

    return 0;
}