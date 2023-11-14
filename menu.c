#include "menu.h"

char *pong_game =
    " /$$$$$$$                                      /$$$$$$                                    \n"
    "| $$__  $$                                    /$$__  $$                                   \n"
    "| $$  \\ $$ /$$$$$$  /$$$$$$$   /$$$$$$       | $$  \\__/  /$$$$$$  /$$$$$$/$$$$   /$$$$$$ \n"
    "| $$$$$$$//$$__  $$| $$__  $$ /$$__  $$      | $$ /$$$$ |____  $$| $$_  $$_  $$ /$$__  $$\n"
    "| $$____/| $$  \\ $$| $$  \\ $$| $$  \\ $$      | $$|_  $$  /$$$$$$$| $$ \\ $$ \\ $$| $$$$$$$$\n"
    "| $$     | $$  | $$| $$  | $$| $$  | $$      | $$  \\ $$ /$$__  $$| $$ | $$ | $$| $$____/ \n"
    "| $$     |  $$$$$$/| $$  | $$|  $$$$$$$      |  $$$$$$/|  $$$$$$$| $$ | $$ | $$|  $$$$$$$\n"
    "|__/      \\______/ |__/  |__/ \\____  $$       \\______/  \\_______/|__/ |__/ |__/ \\_______/\n"
    "                              /$$  \\ $$                                                  \n"
    "                             |  $$$$$$/                                                  \n"
    "                              \\______/                                                   \n";

char *choices[] = {
    "          __                 __   \n"
    "  _______/  |______ ________/  |_ \n"
    " /  ___/\\   __\\__  \\\\_  __ \\   __\\\n"
    " \\___ \\  |  |  / __ \\|  | \\/|  |  \n"
    "/____  > |__| (____  /__|   |__|  \n"
    "     \\/            \\/             \n",
    "              .__  __   \n"
    "  ____ ___  __|__|/  |_ \n"
    "_/ __ \\   \\/  /  \\   __\\\n"
    "\\  ___/ >    <|  ||  |  \n"
    " \\___  >__/\\_ \\__||__|  \n"
    "     \\/      \\/         \n",
};

int n_choices = sizeof(choices) / sizeof(char *);
int startx = WIDTH_MENU / 2;
int starty = HEIGHT_MENU / 10;

WINDOW *create_menu() {
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;
    menu_win = newwin(HEIGHT_MENU, WIDTH_MENU, starty, startx);
    keypad(menu_win, TRUE);
    return menu_win;
}
void highlight_option(WINDOW *menu_win, int highlight) {
    int x, y, i;

    x = 0;
    y = 15;

    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) /* High light the present choice */
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        y += 6;
    }
    wrefresh(menu_win);
}
int print_menu(WINDOW *menu_win) {
    int highlight = 1;
    int choice = 0;
    int c;

    mvwprintw(menu_win, 0, 0, "%s", pong_game);
    highlight_option(menu_win, highlight);
    while (1) {
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                refresh();
                break;
        }
        highlight_option(menu_win, highlight);

        /* User did a choice come out of the infinite loop */
        if (choice != 0) {
            clear();
            return choice;
        }
    }
}