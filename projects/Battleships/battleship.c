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
#define MISS            'X'
#define HIT             'O'
#define EMPTY           '.'
#define FAIL            -1

const bool PLAYING      = true;
const int NO_SHIPS      = 0;
int ALIVE_SHIPS         = 5;

const int VALID_COLUMNS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const char VALID_ROWS[]   = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
const char *ShipTypes[] = {
     "Carrier", "Battleship",
     "Cruiser", "Submarine",
     "Destroyer"
};

enum BATTLESHIPS {
    /* Five initial battleships on board */
    CARRIER=1,
    BATTLESHIP,
    CRUISER,
    SUBMARINE,
    DESTROYER
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
    bool has_ship;
    bool is_hit;
} Cell;

typedef struct {
    ShipType segment;
    Cell ship_body[];
} Segment;

typedef struct Battleship {
    bool is_sunk;
    Segment ship_type;
    char *ship_name;

    bool (*alive)(struct Battleship *self);   // TRUE -> e.g. ship array segment = [1, 1, 1, 1];
    void (*get_position)(struct Battleship *self);
    void (*get_ship_status)(struct Battleship *self);
} Battleship;

struct GridBoard {
    // a board is a grid of Cell
    Cell board[ROWS][COLS];

    void (*print_board)(struct GridBoard*);
};

const Cell CARRIER_SEGMENT[5];
const Cell BATTLEHSIP_SEGMENT[4];
const Cell CRUISER_SEGMENT[3];
const Cell SUBMARINE_SEGMENT[3];
const Cell DESTROYER_SEGMENT[2];

char *toLower(char *s) {
    for (char *p = s; *p; ++p) {
        *p = tolower(*p);
    }

    return s;
}

char *toUpper(char *s) {
    for (char *p = s; *p; ++p) {
        *p = toupper(*p);
    }

    return s;
}

void trim_newline(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

struct Battleship* create_battleship(ShipType battleship_size) {
    // allocate battleship memory
    size_t ship_size = battleship_size.size;
    struct Battleship *new_battleship = malloc(sizeof(struct Battleship) + sizeof(Cell) * ship_size);
    switch (battleship_size.size) {
        case CARRIER:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 5;
            new_battleship->ship_name = "CARRIER";

            // ship coordinates
            new_battleship->ship_type.ship_body[0].row = 'B';
            new_battleship->ship_type.ship_body[0].col = 2;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;

            new_battleship->ship_type.ship_body[1].row = 'C';
            new_battleship->ship_type.ship_body[1].col = 2;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;

            new_battleship->ship_type.ship_body[2].row = 'D';
            new_battleship->ship_type.ship_body[2].col = 2;
            new_battleship->ship_type.ship_body[2].has_ship = true;
            new_battleship->ship_type.ship_body[2].is_hit = false;


            new_battleship->ship_type.ship_body[3].row = 'E';
            new_battleship->ship_type.ship_body[3].col = 2;
            new_battleship->ship_type.ship_body[3].has_ship = true;
            new_battleship->ship_type.ship_body[3].is_hit = false;

            new_battleship->ship_type.ship_body[4].row = 'F';
            new_battleship->ship_type.ship_body[4].col = 2;
            new_battleship->ship_type.ship_body[4].has_ship = true;
            new_battleship->ship_type.ship_body[4].is_hit = false;
            break;
        case BATTLESHIP:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 4;
            new_battleship->ship_name = "BATTLESHIP";

            new_battleship->ship_type.ship_body[0].row = 'A';
            new_battleship->ship_type.ship_body[0].col = 6;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;

            new_battleship->ship_type.ship_body[1].row = 'A';
            new_battleship->ship_type.ship_body[1].col = 7;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;

            new_battleship->ship_type.ship_body[2].row = 'A';
            new_battleship->ship_type.ship_body[2].col = 8;
            new_battleship->ship_type.ship_body[2].has_ship = true;
            new_battleship->ship_type.ship_body[2].is_hit = false;

            new_battleship->ship_type.ship_body[3].row = 'A';
            new_battleship->ship_type.ship_body[3].col = 9;
            new_battleship->ship_type.ship_body[3].has_ship = true;
            new_battleship->ship_type.ship_body[3].is_hit = false;
            break;
        case CRUISER:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 3;
            new_battleship->ship_name = "CRUISER";

            new_battleship->ship_type.ship_body[0].row = 'G';
            new_battleship->ship_type.ship_body[0].col = 9;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;

            new_battleship->ship_type.ship_body[1].row = 'H';
            new_battleship->ship_type.ship_body[1].col = 9;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;

            new_battleship->ship_type.ship_body[2].row = 'I';
            new_battleship->ship_type.ship_body[2].col = 9;
            new_battleship->ship_type.ship_body[2].has_ship = true;
            new_battleship->ship_type.ship_body[2].is_hit = false;
            break;
        case SUBMARINE:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 3;
            new_battleship->ship_name = "SUBMARINE";

            new_battleship->ship_type.ship_body[0].row = 'I';
            new_battleship->ship_type.ship_body[0].col = 2;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;

            new_battleship->ship_type.ship_body[1].row = 'I';
            new_battleship->ship_type.ship_body[1].col = 3;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;

            new_battleship->ship_type.ship_body[2].row = 'I';
            new_battleship->ship_type.ship_body[2].col = 4;
            new_battleship->ship_type.ship_body[2].has_ship = true;
            new_battleship->ship_type.ship_body[2].is_hit = false;
            break;
        case DESTROYER:
            new_battleship->is_sunk = false;
            new_battleship->ship_type.segment.size = 2;
            new_battleship->ship_name = "DESTROYER";

            new_battleship->ship_type.ship_body[0].row = 'J';
            new_battleship->ship_type.ship_body[0].col = 6;
            new_battleship->ship_type.ship_body[0].has_ship = true;
            new_battleship->ship_type.ship_body[0].is_hit = false;

            new_battleship->ship_type.ship_body[1].row = 'J';
            new_battleship->ship_type.ship_body[1].col = 7;
            new_battleship->ship_type.ship_body[1].has_ship = true;
            new_battleship->ship_type.ship_body[1].is_hit = false;
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

bool check_guess(char row_guess, int col_guess) {
    // check(row_guess != NULL, "Row guess is invalid.\n");
    // check(col_guess != NULL, "Column guess is invalid.\n");

    bool v_row = false, v_col = false;
    for (int i = 0; i < ROWS; ++i) {
        if (VALID_ROWS[i] == row_guess)
            v_row = true;
    }
    for (int i = 0; i < COLS; ++i) {
        if (VALID_COLUMNS[i] == col_guess)
            v_col = true;
    }
    return (v_row && v_col);
error:
    printf("<error print> Invalid coordinate");
    return false;
}

bool is_valid_guess(Guess *coordinate) {
    char row_guess = coordinate->row;
    int  col_guess = coordinate->col;

    if (check_guess(row_guess, col_guess) == true)
        return true;
    return false;
}

// FIXME
void (*update_board_on_guess(struct GridBoard *game_board, Guess *coordinate))() {
    /* validate player guess */
    if (!is_valid_guess(coordinate)) {
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

// FIXME
void get_current_ship_position(Battleship *self) {
    // printf("The %s ship is at position (%d,%d)\n",
    //        self->ship_name,
    //        self->cell.row, self->cell.col);
}

void display_board(struct GridBoard* grid_board) {
    // columns (1-10)
    printf("   ");
    for (int j = 1; j <= COLS; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    // rows (A-J)
    for (int i = 0; i < ROWS; i++) {//
        printf("%c  ", 'A' + i);
        for (int j = 0; j < COLS; j++) {
            char board_coordinate = EMPTY;
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

void initiate_board(struct GridBoard* grid_board) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid_board->board[i][j] = (Cell){i, j, false, false};
        }
    }

    display_board(grid_board);
}

// TOOD
void place_ship(struct GridBoard *game_board, Battleship *ship) {
    size_t ship_size = ship->ship_type.segment.size;
    for (size_t i = 0; i < ship_size; ++i) {
        Cell *body = &ship->ship_type.ship_body[i];
        // convert letter row to index (A->0, B->1)...
        int row_index = body->row - 'A';
        int col_index = body->col - 1;

        // update board
        game_board->board[row_index][col_index].has_ship = true;
    }
}

bool process_guess(struct GridBoard *game_board, Battleship *ships[], Guess *coordinate) {
    int row_index = coordinate->row - 'A';
    int col_index = coordinate->col - 1;

    game_board->board[row_index][col_index].is_hit = true;

    if (game_board->board[row_index][col_index].has_ship) {
        for (int i = 0; i < MAX_SIZE; ++i) {
            Battleship *ship = ships[i];
            for (size_t j = 0; j < ship->ship_type.segment.size; ++j) {
                Cell *body = &ship->ship_type.ship_body[j];
                if (body->row - 'A' == row_index && body->col - 1 == col_index) {
                    body->is_hit = true;

                    // check if ship sunk
                    bool is_sunk = true;
                    for (size_t k = 0; k < ship->ship_type.segment.size; ++k) {
                        if (!ship->ship_type.ship_body[k].is_hit) {
                            is_sunk = false;
                            break;
                        }
                    }
                    if (is_sunk && !ship->is_sunk) {
                        ship->is_sunk = true;
                        ALIVE_SHIPS--;
                        printf("You sunk the %s\n", ship->ship_name);
                    }

                    // hit
                    return true;
                }
            }
        }
    }

    // miss
    return false;
}

// TODO
char *attack(Cell attack_coordinate) {
    return "HIT";
}

bool play_game_true(char *str) {
    return (strcmp(str, "yes") == 0);
}

bool play_game_false(char *str) {
    return (strcmp(str, "no") == 0);
}

void start_game(struct GridBoard game_board) {
    printf("\n-----------------------------BattleShips-----------------------------\n");
    printf("Battleships is a strategy and guessing-type game that involves..well\n");
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

    char input[MAX_DATA];
    printf("Are you ready to play the Battleships game? (yes or no): ");
    fgets(input, sizeof(input), stdin); trim_newline(input);

    while (!play_game_true(input) && !play_game_false(input)) {
        printf("Please enter either 'yes' or 'no'.\n");
        fgets(input, sizeof(input), stdin); trim_newline(input);
    }

    if (play_game_true(input)) {

        printf("\n");
        game_board.print_board = initiate_board;
        game_board.print_board(&game_board);
        game_board.print_board = display_board;

        // create ships
        Battleship *ships[MAX_SIZE];
        ships[0] = create_battleship((ShipType){5}); // Carrier
        ships[1] = create_battleship((ShipType){4}); // Battleship
        ships[2] = create_battleship((ShipType){3}); // Cruiser
        ships[3] = create_battleship((ShipType){3}); // Submarine
        ships[4] = create_battleship((ShipType){2}); // Destroyer

        // place ships on board
        for (int i = 0; i < MAX_SIZE; i++) {
            place_ship(&game_board, ships[i]);
        }

        // main loop
        while (ALIVE_SHIPS > NO_SHIPS) {
            sleep(1);
            printf("There remains %d ships!\n", ALIVE_SHIPS);

            // read guess
            Guess player_guess;
            char buf[MAX_DATA];

            printf("Entering coordinate..\n");
            printf("Row (A-J): ");
            fgets(buf, sizeof(buf), stdin);
            player_guess.row = toupper(buf[0]);

            printf("Column (1-10): ");
            fgets(buf, sizeof(buf), stdin);
            player_guess.col = atoi(buf);

            printf("You entered %c%d.\n", player_guess.row, player_guess.col);

            // validate guess
            if (!is_valid_guess(&player_guess)) {
                printf("Invalid coordinate. Please try again.\n");
                continue;
            }

            // check guess and update board
            bool hit = process_guess(&game_board, ships, &player_guess);
            if (hit) {
                printf("HIT!\n");
            } else {
                printf("MISS!\n");
            }

            // display board
            game_board.print_board(&game_board);

            // check game over condition
            if (ALIVE_SHIPS == NO_SHIPS) {
                printf("Congratulations! You've sunk all ships!\n");
                break;
            }
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
