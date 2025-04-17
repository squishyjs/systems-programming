#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#define WIDTH 20
#define HEIGHT 10

// Read a single character without needing to press enter
char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);            // Get current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);          // Disable buffering & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);   // Apply new settings
    ch = getchar();                            // Read character
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);   // Restore settings
    return ch;
}

// Clear the screen
void clearScreen() {
    printf("\033[H\033[J"); // ANSI escape code: home + clear screen
}

int main() {
    int x = WIDTH / 2;
    int y = HEIGHT / 2;
    char input;

    while (1) {
        clearScreen();

        // Draw game world
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (i == y && j == x)
                    printf("@"); // Player
                else
                    printf(".");
            }
            printf("\n");
        }

        printf("Use WASD to move, Q to quit.\n");

        input = getch();
        if (input == 'q' || input == 'Q') break;
        if (input == 'w' && y > 0) y--;
        if (input == 's' && y < HEIGHT - 1) y++;
        if (input == 'a' && x > 0) x--;
        if (input == 'd' && x < WIDTH - 1) x++;
    }

    clearScreen();
    printf("Game Over!\n");
    return 0;
}
