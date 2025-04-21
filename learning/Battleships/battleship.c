#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS  10
#define COLS  10
#define MISS  'X'
#define HIT   'O'

typedef struct {
    int row;
    int col;
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
    //constructing a segment is terminated

    void (*print_board)(struct GridBoard*);
};

void print_ship_status(Battleship *self) {
    if (self->is_sunk) {
        printf("The %s ship has sunk!\n", self->ship_name);
    } else {
        printf("The %s ship is still in battle.\n", self->ship_name);
    }
}

void get_current_ship_position(Battleship *self) {
    printf("The %s ship is at position (%d,%d)\n",
           self->ship_name,
           self->cell.row, self->cell.col);
}

void display_board(struct GridBoard* grid_board) {
    //initial empty board -> all dots
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid_board->board[i][j] = (Cell){i, j, false, false};
        }
    }
    //columns (1 -> 10)
    printf("   ");
    for (int j = 1; j <= COLS; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    //rows (A -> J)
    for (int i = 0; i < ROWS; i++) {
        printf("%c  ", 'A' + i);
        for (int j = 0; j < COLS; j++) {
            char board_coordinate = '.';
            if (grid_board->board[i][j].is_hit) {
                if (grid_board->board[i][j].has_ship) {
                    board_coordinate = HIT;
                } else {
                    board_coordinate = MISS;
                }
            }
            printf(" %c ", board_coordinate);
        }
        printf("\n");
    }
}


int main(void) {
    // struct GridBoard board;
    // board.print_board = &display_board();
    struct GridBoard board;
    board.print_board = display_board;
    
    board.print_board(&board);

    return 0;
}
