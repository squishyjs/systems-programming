#include <ncurses.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void) {

    initscr();
    printw("Hello World!!");

    refresh();
    getch();

    endwin();

    return EXIT_SUCCESS;
}
