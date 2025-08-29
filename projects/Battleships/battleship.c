#include "dbg.h"

#include <ctype.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define F               fflush(stdout)
#define ROWS            10
#define COLS            10
#define MAX_DATA        100
#define MAX_SIZE        5
#define SHIP            'S'
#define MISS            'X'
#define HIT             'O'
#define EMPTY           '.'
#define FAIL            -1

const bool PLAYING            = true;
const int NUMBER_OF_SHIPS     = 5;
const int NO_SHIPS            = 0;
volatile int SHIPS_ALIVE      = 5;

const int VALID_COLUMNS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const char VALID_ROWS[]   = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
const char *ShipTyes[] = {
     "Carrier", "Battleship",
     "Cruiser", "Submarine",
     "Destroyer"
};

enum BATTLESHIPS {
    // battleship types
    CARRIER=1,
    BATTLESHIP,
    CRUISER,
    SUBMARINE,
    DESTROYER
};

typedef enum {
    EMPTY_CELL,
    SHIP_CELL,
    MISS_CELL,
    HIT_CELL,
    GUESSED_CELL
} CellState;

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
    char cell;
    bool has_ship;
    bool is_hit;
    bool guessed;
} Cell;

typedef struct {
    ShipType segment;
    Cell ship_body[]; // length is determined at compile time (malloc runtime)
} Segment;


typedef struct Battleship {
    bool (*alive)(struct Battleship *self);     // true -> e.g. ship array segment = [1, 1, 1, 1];
    void (*get_position)(struct Battleship *self);
    void (*get_ship_status)(struct Battleship *self);

    char *ship_name;
    int health;
    bool is_sunk;
    Segment ship_type;
} Battleship;

struct GridBoard {
    Cell board[ROWS][COLS];                     // board is a grid of Cell
    void (*print_board)(struct GridBoard*);
};

const Cell CARRIER_SEGMENT[5];
const Cell BATTLEHSIP_SEGMENT[4];
const Cell CRUISER_SEGMENT[3];
const Cell SUBMARINE_SEGMENT[3];
const Cell DESTROYER_SEGMENT[2];

char *toUpper(char *str) {
    for (char *p = str; *p; ++p) {
        *p = toupper(*p);
    }

    return str;
}

char *toLower(char *str) {
    for (char *p = str; *p; ++p) {
        *p = tolower(*p);
    }

    return str;
}

void trim_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

struct Battleship* create_battleship(ShipType battleship_size) {
    // allocate memory for battlehsip
    size_t ship_size = battleship_size.size;
    struct Battleship *new_battleship = malloc(sizeof(struct Battleship) + sizeof(Cell) * ship_size);
    switch (battleship_size.size) {
        case CARRIER:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 5;
            new_battleship->ship_name = "CARRIER";
            new_battleship->health = new_battleship->ship_type.segment.size;

            // ship coordinates
            new_battleship->ship_type.ship_body[0].row = 'B';
            new_battleship->ship_type.ship_body[0].col = 2;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;
            new_battleship->ship_type.ship_body[0].guessed = false;
            new_battleship->ship_type.ship_body[0].cell = SHIP;

            new_battleship->ship_type.ship_body[1].row = 'C';
            new_battleship->ship_type.ship_body[1].col = 2;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;
            new_battleship->ship_type.ship_body[1].guessed = false;
            new_battleship->ship_type.ship_body[1].cell = SHIP;

            new_battleship->ship_type.ship_body[2].row = 'D';
            new_battleship->ship_type.ship_body[2].col = 2;
            new_battleship->ship_type.ship_body[2].has_ship = true;
            new_battleship->ship_type.ship_body[2].is_hit = false;
            new_battleship->ship_type.ship_body[2].guessed = false;
            new_battleship->ship_type.ship_body[2].cell = SHIP;

            new_battleship->ship_type.ship_body[3].row = 'E';
            new_battleship->ship_type.ship_body[3].col = 2;
            new_battleship->ship_type.ship_body[3].has_ship = true;
            new_battleship->ship_type.ship_body[3].is_hit = false;
            new_battleship->ship_type.ship_body[3].guessed = false;
            new_battleship->ship_type.ship_body[3].cell = SHIP;

            new_battleship->ship_type.ship_body[4].row = 'F';
            new_battleship->ship_type.ship_body[4].col = 2;
            new_battleship->ship_type.ship_body[4].has_ship = true;
            new_battleship->ship_type.ship_body[4].is_hit = false;
            new_battleship->ship_type.ship_body[4].guessed = false;
            new_battleship->ship_type.ship_body[4].cell = SHIP;
            break;
        case BATTLESHIP:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 4;
            new_battleship->ship_name = "BATTLESHIP";
            new_battleship->health = new_battleship->ship_type.segment.size;

            new_battleship->ship_type.ship_body[0].row = 'A';
            new_battleship->ship_type.ship_body[0].col = 6;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;
            new_battleship->ship_type.ship_body[0].guessed = false;
            new_battleship->ship_type.ship_body[0].cell = SHIP;

            new_battleship->ship_type.ship_body[1].row = 'A';
            new_battleship->ship_type.ship_body[1].col = 7;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;
            new_battleship->ship_type.ship_body[1].guessed = false;
            new_battleship->ship_type.ship_body[1].cell = SHIP;

            new_battleship->ship_type.ship_body[2].row = 'A';
            new_battleship->ship_type.ship_body[2].col = 8;
            new_battleship->ship_type.ship_body[2].has_ship = true;
            new_battleship->ship_type.ship_body[2].is_hit = false;
            new_battleship->ship_type.ship_body[2].guessed = false;
            new_battleship->ship_type.ship_body[2].cell = SHIP;

            new_battleship->ship_type.ship_body[3].row = 'A';
            new_battleship->ship_type.ship_body[3].col = 9;
            new_battleship->ship_type.ship_body[3].has_ship = true;
            new_battleship->ship_type.ship_body[3].is_hit = false;
            new_battleship->ship_type.ship_body[3].guessed = false;
            new_battleship->ship_type.ship_body[3].cell = SHIP;
            break;
        case CRUISER:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 3;
            new_battleship->ship_name = "CRUISER";
            new_battleship->health = new_battleship->ship_type.segment.size;

            new_battleship->ship_type.ship_body[0].row = 'G';
            new_battleship->ship_type.ship_body[0].col = 9;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;
            new_battleship->ship_type.ship_body[0].guessed = false;
            new_battleship->ship_type.ship_body[0].cell = SHIP;

            new_battleship->ship_type.ship_body[1].row = 'H';
            new_battleship->ship_type.ship_body[1].col = 9;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;
            new_battleship->ship_type.ship_body[1].guessed = false;
            new_battleship->ship_type.ship_body[1].cell = SHIP;

            new_battleship->ship_type.ship_body[2].row = 'I';
            new_battleship->ship_type.ship_body[2].col = 9;
            new_battleship->ship_type.ship_body[2].has_ship = true;
            new_battleship->ship_type.ship_body[2].is_hit = false;
            new_battleship->ship_type.ship_body[2].guessed = false;
            new_battleship->ship_type.ship_body[2].cell = SHIP;
            break;
        case SUBMARINE:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 3;
            new_battleship->ship_name = "SUBMARINE";
            new_battleship->health = new_battleship->ship_type.segment.size;

            new_battleship->ship_type.ship_body[0].row = 'I';
            new_battleship->ship_type.ship_body[0].col = 2;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;
            new_battleship->ship_type.ship_body[0].guessed = false;
            new_battleship->ship_type.ship_body[0].cell = SHIP;

            new_battleship->ship_type.ship_body[1].row = 'I';
            new_battleship->ship_type.ship_body[1].col = 3;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;
            new_battleship->ship_type.ship_body[1].guessed = false;
            new_battleship->ship_type.ship_body[1].cell = SHIP;

            new_battleship->ship_type.ship_body[2].row = 'I';
            new_battleship->ship_type.ship_body[2].col = 4;
            new_battleship->ship_type.ship_body[2].has_ship = true;
            new_battleship->ship_type.ship_body[2].is_hit = false;
            new_battleship->ship_type.ship_body[2].guessed = false;
            new_battleship->ship_type.ship_body[2].cell = SHIP;
            break;
        case DESTROYER:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 2;
            new_battleship->ship_name = "DESTROYER";
            new_battleship->health = new_battleship->ship_type.segment.size;

            new_battleship->ship_type.ship_body[0].row = 'J';
            new_battleship->ship_type.ship_body[0].col = 6;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;
            new_battleship->ship_type.ship_body[0].guessed = false;
            new_battleship->ship_type.ship_body[0].cell = SHIP;

            new_battleship->ship_type.ship_body[1].row = 'J';
            new_battleship->ship_type.ship_body[1].col = 7;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;
            new_battleship->ship_type.ship_body[1].guessed = false;
            new_battleship->ship_type.ship_body[1].cell = SHIP;
            break;
        default:
            sentinel("Invalid ship type: %zu. Please enter a VALID ship size: (Carrier = 1, Battleship = 2, Cruiser = 3, Submarine = 4, Destroyer = 5)\n", battleship_size.size);
            break;
    };

    return new_battleship;
error:
    if (new_battleship)
        free(new_battleship);
    return NULL;
}

void print_hit_status(Battleship *self) {
    printf("Hit!\n");
    if (self->is_sunk) {
        printf("The %s ship has sunk!\n", self->ship_name);
    } else {
        printf("The %s ship is in battle!\n", self->ship_name);
    }
}

void print_miss_status() {
    printf("Miss. Better luck next time!\n");
}

bool alive_ship(Battleship *ship) {
    return ship->alive;
}

void print_sunk_status(char *ship_name) {
    printf("Hit!\n");
    printf("You sunk the %s!\n", ship_name);
}

void display_board(struct GridBoard* grid_board) {
    // columns (1-10)
    printf("   ");
    for (int j = 1; j <= COLS; j++) {
        printf("%2d ", j);
    }

    printf("\n");

    for (int row = 0; row < ROWS; row++) {
        printf("%c  ", 'A' + row);

        for (int col = 0; col < COLS; col++) {
            char board_cell = EMPTY;

            // switch cell state
            if (grid_board->board[row][col].is_hit
                && grid_board->board[row][col].has_ship) {
                board_cell = HIT;
            } else if (grid_board->board[row][col].guessed) {
                board_cell = MISS;
            } else {
                board_cell = EMPTY;
            }

            // update cell state
            grid_board->board[row][col].cell = board_cell;
            printf(" %c ", board_cell);
        }

        printf("\n");
    }
}

void initiate_board(struct GridBoard* grid_board) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid_board->board[i][j] = (Cell){i, j, EMPTY, false, false, false};
        }
    }

    // display_board(grid_board);
}

void place_ship(struct GridBoard *game_board, Battleship *ship) {
    size_t ship_size = ship->ship_type.segment.size;
    for (size_t i = 0; i < ship_size; ++i) {
        Cell *body = &ship->ship_type.ship_body[i];
        // convert letter row to index (A->0, B->1)...
        int row_index = body->row - 'A';
        int col_index = body->col - 1;

        // default cell that has ship
        game_board->board[row_index][col_index].has_ship = true;
        game_board->board[row_index][col_index].is_hit = false;
        game_board->board[row_index][col_index].guessed = false;
        game_board->board[row_index][col_index].cell = SHIP;
    }
}

bool is_valid_guess(struct GridBoard *game_board, Guess *coordinate) {
    char row_guess = toupper(coordinate->row);
    int  col_guess = coordinate->col;

    if (row_guess < 'A' || row_guess > 'J') {
        return false;
    }

    if (col_guess < 1 || col_guess > 10) {
        return false;
    }

    int r = row_guess - 'A';
    int c = col_guess - 1;
    // check if already guessed
    if (game_board->board[r][c].guessed || game_board->board[r][c].is_hit) {
        printf("You have already guessed this coordinate. Please try again!\n");
        return false;
    }

    return true;
}

void process_guess(struct GridBoard *game_board, Battleship *ships[], Guess *coordinate) {
    int row = coordinate->row - 'A';
    int col = coordinate->col - 1;

    // miss
    if (!game_board->board[row][col].has_ship) {
        game_board->board[row][col].cell = MISS;
        game_board->board[row][col].guessed = true;
        printf("Miss!\n");
        return;
    }

    // already hit
    if (game_board->board[row][col].is_hit) {
        printf("You have already shot here!\n");
        return;
    }

    // update cell state
    game_board->board[row][col].is_hit = true;
    game_board->board[row][col].guessed = true;
    game_board->board[row][col].cell = HIT;

    // TODO: check WHICH SHIP was hit and update the Cell of its segment to reflect 'hit' state
    for (int i = 0; i < NUMBER_OF_SHIPS; ++i) {
        Battleship curr_ship = *ships[i];
        for (int j = 0; j < curr_ship.ship_type.segment.size; ++j) {
            // check if current cell of ship segment were the
            // coordinates of the (row, col) player guess
            if (ships[i]->ship_type.ship_body[j].row == coordinate->row
                && ships[i]->ship_type.ship_body[j].col == coordinate->col) {
                ships[i]->ship_type.ship_body[j].is_hit = true;
                ships[i]->ship_type.ship_body[j].guessed = true;
                ships[i]->ship_type.ship_body[j].cell = HIT;

                ships[i]->health--;
                if (ships[i]->health <= 0) {
                    ships[i]->is_sunk = true;
                    SHIPS_ALIVE--;

                    print_sunk_status(curr_ship.ship_name);
                    return;
                }
            }
        }
    }

    printf("Hit!\n");
}

// FIXME: board not being updated properly, need to fix
void (*update_board_on_guess(struct GridBoard *game_board, Guess *coordinate))() {
    /* validate player guess */
    if (!is_valid_guess(game_board, coordinate)) {
        printf("Please enter a valid guess");
        exit(FAIL);
    }

    // check coordinate of player guess
    bool hit = false;
    int row_guess = coordinate->row;
    char col_guess = coordinate->col;
    // begin for loop over board
    for (int row = 0; row < ROWS; ++row) {
        for (int column = 0; column < COLS; ++column) {
            if (row == row_guess && column == col_guess) {

                Cell *current_cell = &game_board->board[row][column];
                if (current_cell->has_ship)
                    hit = true;
            }
        }
    }

    return hit ? &print_hit_status : &print_miss_status;
}

Guess read_guess(void) {
    char buf[MAX_DATA];
    Guess guess;

    printf("Enter coordinate (A-J, 1-10)\n");

    // row
    printf("Row: ");
    if (!fgets(buf, sizeof(buf), stdin)) {
        printf("Error.\n");
    }
    trim_newline(buf);
    guess.row = toupper(buf[0]);

    // column
    printf("Col: ");
    if (!fgets(buf, sizeof(buf), stdin)) {
        printf("Error.\n");
    }
    trim_newline(buf);
    guess.col = atoi(buf);

    return guess;
}

// TODO: need to finish function
char *attack(Cell attack_coordinate) {
    return "HIT";
}

bool play(char *str) {
    return (strcmp(toLower(str), "yes") == 0);
}

bool not_play(char *str) {
    return (strcmp(toLower(str), "no") == 0);
}

bool valid_play(char *str) {
    return (play(str) || not_play(str));
}

void start_game(struct GridBoard game_board) {
    printf("\n--------------------------BattleShips--------------------------------\n");
    printf("Battleships is a strategy and guessing-type game that involves..well\n");
    printf("battleships. A Grid board of 10 x 10 squares is displayed, with rows\n");
    printf("of A-J, and columns numbered 1-10. In each move, you need to guess\n");
    printf("where a battleship may be. For instance, picking a coordinate (x, y)\n");
    printf("may result in either a hit (X) or miss (O). The goal is then to find\n");
    printf("where all five ships are located, and SINK THEM!. The five ships are:\n\n");
    printf("                         1. Carrier\n");
    printf("                         2. Battleship\n");
    printf("                         3. Cruiser\n");
    printf("                         4. Submarine\n");
    printf("                         5. Destroyer\n");
    printf("\n---------------------------------------------------------------------\n");

    char input[MAX_DATA];
    printf("Are you ready to play the Battleships game? ('YES' or 'NO'): ");
    fgets(input, sizeof(input), stdin);
    trim_newline(input);

    while (!valid_play(input)) {
        printf("Please enter either 'yes' or 'no'.\n");
        fgets(input, sizeof(input), stdin);

        trim_newline(input);
    }

    if (play(input)) {

        printf("\n");
        game_board.print_board = initiate_board;
        game_board.print_board(&game_board);
        game_board.print_board = display_board;

        // create ships
        Battleship *ships[NUMBER_OF_SHIPS];
        ships[0] = create_battleship((ShipType){CARRIER});
        ships[1] = create_battleship((ShipType){BATTLESHIP});
        ships[2] = create_battleship((ShipType){CRUISER});
        ships[3] = create_battleship((ShipType){SUBMARINE});
        ships[4] = create_battleship((ShipType){DESTROYER});

        // place ships on board
        for (int i = 0; i < NUMBER_OF_SHIPS; i++) {
            place_ship(&game_board, ships[i]);
        }

        display_board(&game_board);

        // main loop
        while (SHIPS_ALIVE > 0) {
            printf("There remains %d ships!\n", SHIPS_ALIVE);
            sleep(1);

            // read guess
            Guess player_guess = read_guess();
            while (!is_valid_guess(&game_board, &player_guess)) {
                player_guess = read_guess();
            }

            // TODO: process valid guess
            process_guess(&game_board, ships, &player_guess);

            // display board
            display_board(&game_board);
        }

        // game over state
        if (SHIPS_ALIVE == NO_SHIPS) {
            printf("Congratulations! You've sunk all ships!\n");
        }

        // free memory
        for (int i = 0; i < MAX_SIZE; i++) {
            free(ships[i]);
        }

    } else {
        printf("Goodbye!\n");
    }

}

int main(void) {

    struct GridBoard clean_board;
    start_game(clean_board);
    return 0;

}
