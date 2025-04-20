#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS  10
#define COLS  10
#define MISS  'x'
#define HIT   'o'

typedef struct {
    int *x_pos;
    int *y_pos;
    bool has_ship;   // TRUE -> occupied by ship
    bool is_hit;     // TRUE -> marked by 'o'
} Cell;

typedef struct Battleship {
    char *ship_name;
    Cell cell;
    bool is_sunk;
    void (*get_position)(struct Battleship *self);
    void (*get_ship_status)(struct Battleship *self);
} Battleship;

struct GridBoard {
    /* board has a list of battleships */
    Cell board[ROWS][COLS];
    //probably need to allocate some memory for the ships and place 
    //them in an array remove them from the array if all its cells 
    //making its segment is terminated

    void (*print_board)(char *board);
};

void print_ship_status(Battleship *self) {
    if (self->is_sunk) {
        printf("The %s ship has sunk!\n", self->ship_name);
    } else {
        printf("The %s ship is still in battle.\n", self->ship_name);
    }
}

void get_current_ship_position(Battleship *self) {
    printf("The %s ship is at position [%d][%d]\n",
           self->ship_name,
           self->cell->x_pos, self->cell->y_pos);
}

void display_board(struct GridBoard board) {
    //initial empty board -> all dots
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            //TODO: refactor each cell in the board to be of struct "Cell"
            Cell cell = {&i, &j, false, false};
            board[i][j] = '.';
        }
    }
    //column headers (1 -> 10)
    printf("   ");
    for (int j = 1; j <= COLS; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    //row labels (A -> J)
    for (int i = 0; i < ROWS; i++) {
        printf("%c  ", 'A' + i);
        for (int j = 0; j < COLS; j++) {
            printf(" %c ", board[i][j]);
        }
        printf("\n");
    }
}


int main(void) {
    

    return 0;
}
