#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define WIDTH 20
#define HEIGHT 8

const char UP    = 'W';
const char DOWN  = 'S';
const char RIGHT = 'D';
const char LEFT  = 'A';
const char QUIT  = 'Q';
volatile bool GAME  = true;              //interpolated as '1'

int x_pos = HEIGHT / 2;         //middle position
int y_pos = (WIDTH / 2) - 1;    //'-1' for offset to centre

int main(void) {
    // char buf[100];
    // printf("Enter your name: ");
    // fgets(buf, sizeof(buf), stdin);
    // printf(buf);
    // int i = 0;
    while (GAME) {
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if ( (i == x_pos) && (j == y_pos)) {
                    write(STDOUT_FILENO, "@", 1);
                    continue;
                }
                write(STDOUT_FILENO, ".", 1);
            }
            printf("\n");
        }
        write(STDOUT_FILENO, "Use WASD to move, Q to quit.\n", 30);
        
        char move_buf;
        move_buf = toupper(getchar());
        if (move_buf == UP && x_pos > 0) {
            puts("You went up!");
            x_pos--;
        } else if (move_buf == DOWN && x_pos < HEIGHT - 1) {
            puts("You went down!");
            x_pos++;
        } else if (move_buf == RIGHT && y_pos < WIDTH - 1) {
            puts("You went right!");
            y_pos++;
        } else if (move_buf == LEFT && y_pos > 0) {
            puts("You went left!");
            y_pos--;
        } else if (move_buf == QUIT) {
            puts("See you soon!");
            GAME = false;
            break;
        } else {
            puts("Please enter a valid move: WASD.\n");
        }

        // printf("%d\n", i);
        // ++i;
    }

    return 0;
}
