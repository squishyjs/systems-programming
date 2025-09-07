#include <raylib.h>

#include <stdbool.h>
#include <unistd.h>
#include <unistdio.h>
#include <stdio.h>
#include <string.h>

#define MAX_BULLETS 10000
#define LIVES       3

static const int screen_width = 800;
static const int screen_height = 450;
static const int fps_rate = 120;
static const char *window_title = "asteroids";

static void update_game(void);
static void draw_game(void);
static void update_draw_game(void);


typedef struct {
    Vector2 position;
    int lives;
    int bullets[MAX_BULLETS];
} Ship;

void update_game(void) {
    // TODO:
}

void draw_game(void) {
    // TODO:
}

void update_draw_game(void) {

    // render single frame
    update_game();
    draw_game();

}

int main(void) {

    InitWindow(screen_width, screen_height, window_title);
    SetTargetFPS(fps_rate);

    while (!WindowShouldClose()) {
        // main game loop
        update_draw_game();
    }

    CloseWindow();

    return 0;
}
