#include "dbg.h"

#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define FAIL        -1
#define ROWS        10
#define COLS        10
#define MAX_SIZE    5
#define MISS        'X'
#define HIT         'O'

enum BATTLESHIPS {
    /* Five initial battleships on board */
    CARRIER=1,
    BATTLESHIP,
    CRUISER,
    SUBMARINE,
    DESTROYER
};

const int VALID_COLUMNS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const char VALID_ROWS[]   = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
const char *ShipTypes[] = {
     "Carrier", "Battleship", 
     "Cruiser", "Submarine",
     "Destroyer"
};

typedef struct Guess {
    char row;
    int col;
} Guess;

typedef struct {
    size_t size;
} ShipType;

typedef struct {
    int row;
    int col;
    bool has_ship;              // TRUE -> occupied by ship
    bool is_hit;                // TRUE -> marked by 'O'
} Cell;

typedef struct {
    /* an array of board Cells (vertical or horizontal) */
    ShipType segment;
    char body[];                // dynamically allocated
} Segment;

//TODO: Implement specific battleship types
typedef struct Battleship {
    char *ship_name;
    Segment ship_type;
    bool is_sunk;               // TRUE -> ship_health is dead

    bool (*alive)(struct Battleship *self);   //TRUE -> e.g. ship array segment = [1, 1, 1, 1];
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

struct Battleship* create_battleship(ShipType battleship_size) {
    Battleship new_battleship;
    switch (battleship_size.size) {
        case CARRIER:
            new_battleship.is_sunk = false;
            new_battleship.ship_type.segment.size = 5;
            new_battleship.ship_name = "CARRIER";
            break;
        case BATTLESHIP:
            new_battleship.is_sunk = false;
            new_battleship.ship_type.segment.size = 4;
            new_battleship.ship_name = "BATTLESHIP";
            break;
        case CRUISER:
            new_battleship.is_sunk = false;
            new_battleship.ship_type.segment.size = 3;
            new_battleship.ship_name = "CRUISER";
            break;
        case SUBMARINE:
            new_battleship.is_sunk = false;
            new_battleship.ship_type.segment.size = 3;
            new_battleship.ship_name = "SUBMARINE";
            break;
        case DESTROYER:
            new_battleship.is_sunk = false;
            new_battleship.ship_type.segment.size = 2;
            new_battleship.ship_name = "DESTROYER";
            break;
        default:
            perror("Please enter a VALID ship size: (Carrier = 1, Battleship = 2, Cruiser = 3, Submarine = 4, Destroyer = 5)\n");
            break;
    };
    //TODO: need to malloc some shit

    return &new_battleship;
}

void print_hit_status(Battleship *self) {
    if (self->is_sunk) {
        printf("The %s ship has sunk!\n", self->ship_name);
    } else {
        printf("The %s ship is still in battle.\n", self->ship_name);
    }
}

void print_miss_status() {
    printf("Shot Missed! Try again.\n");
}

bool is_valid_guess(char *row_guess, int*col_guess) {
    check(row_guess != NULL, "Row guess is invalid.\n");
    check(col_guess != NULL, "Column guess is invalid.\n");

    bool v_row = false, v_col = false;
    for (int i = 0; i < ROWS; ++i) {
        if (VALID_ROWS[i] == (*row_guess))
            v_row = true;
    }
    for (int i = 0; i < COLS; ++i) {
        if (VALID_COLUMNS[i] == (*col_guess))
            v_col = true;
    }
    return (v_row && v_col) ? true : false;
error:
    printf("<error print> Invalid guess. Returning 'false'");
    return false;
}

bool check_guess(Guess *coordinate) {
    char  row_guess = coordinate->row;
    int col_guess = coordinate->col;
    //TODO: validate guess
    if (is_valid_guess(&row_guess, &col_guess))
        return true;
    return false;
}

//TODO: refresh board after player (x, y) guess
void (*update_board_on_guess(struct GridBoard *game_board, Guess *coordinate))() {
    /* validate player guess */
    if (!check_guess(coordinate)) {
        printf("Please enter a valid guess");
        exit(FAIL);
    }

    //check coordinate of player guess
    bool successful_hit = false;
    int  row_player_guess = coordinate->row;
    char col_player_guess = coordinate->col;
    //begin for loop over board
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (i == row_player_guess && j == col_player_guess) {
                //current cell should point to cell in game_board
                //need to check if some FUNNY BUSINESS IS GOING ON HERE:
                Cell *current_cell = &game_board->board[i][j];
                /* check if cell contains a ship segment */
                if (current_cell->has_ship)
                    successful_hit = true;
            }
        }
    }
    return successful_hit ? &print_hit_status : &print_miss_status;
}

//TODO: function should use Segment ! Cell
void get_current_ship_position(Battleship *self) {
    printf("The %s ship is at position (%d,%d)\n",
           self->ship_name,
           self->cell.row, self->cell.col);
}

/* TODO: need to split function between initialising the board, and displaying it */
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
    for (int i = 0; i < ROWS; i++) {//
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
//TODO: place ship -> board
void place_ship(struct GridBoard *game_board, Battleship *ship) {
    return;
}

//TODO: return a string per se
char *attack(Cell attack_coordinate) {
    return "HIT";
}

void start_game(struct GridBoard game_board) {
    game_board.print_board = display_board;
    game_board.print_board(&game_board);

    printf("\n-----------------------------BattleShips-----------------------------\n");
    printf("Battleshisps is a strategy and guessing-type game that involves..well\n");
    printf("battleships. A Grid board of 10 x 10 squares is displayed, with rows\n");
    printf("of A-J, and columns numbered 1-10. In each move, you need to guess\n");
    printf("where a battleship may be. For instance, picking a coordinate (x, y)\n");
    printf("may result in either a hit (X) or miss (O). The goal is then to find\n");
    printf("where all five ships are located, and SINK THEM!. The five ships are:\n\n");
    printf("                            1. Carrier\n");
    printf("                            2. Battleship\n");
    printf("                            3. Cruiser\n");
    printf("                            4. Submarine\n");
    printf("                            5. Destroyer\n");
    printf("\n---------------------------------------------------------------------\n");
    printf("Are you ready to play the Battleships game? (yes or no): ");
    
}

int main(void) {
    //create a new board
    struct GridBoard clean_board;
    start_game(clean_board);
    return 0;
}

