#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define WIDTH 20
#define HEIGHT 8

const char UP    = 'W';
const char DOWN  = 'S';
const char RIGHT = 'D';
const char LEFT  = 'A';
const char QUIT  = 'Q';

volatile bool GAME = true;

int x_pos = HEIGHT / 2;
int y_pos = (WIDTH / 2) - 1;

// ---- Terminal control ---- //
struct termios orig_termios;

void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);  // turn off echo & canonical mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// ---- Screen control ---- //
void clear_screen(void) {
    printf("\033[H\033[J");  // move cursor home + clear
    fflush(stdout);
}

// ---- Game ---- //
int main(void) {
    enable_raw_mode();

    while (GAME) {
        clear_screen();

        // Draw grid
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (i == x_pos && j == y_pos) {
                    printf("@");
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }

        printf("Use WASD to move, Q to quit.\n");

        // Read one key instantly
        char move_buf;
        if (read(STDIN_FILENO, &move_buf, 1) == -1) continue;
        move_buf = toupper(move_buf);

        if (move_buf == UP && x_pos > 0) {
            x_pos--;
        } else if (move_buf == DOWN && x_pos < HEIGHT - 1) {
            x_pos++;
        } else if (move_buf == RIGHT && y_pos < WIDTH - 1) {
            y_pos++;
        } else if (move_buf == LEFT && y_pos > 0) {
            y_pos--;
        } else if (move_buf == QUIT) {
            clear_screen();
            printf("See you soon!\n");
            GAME = false;
        }
    }

    return 0;
}
