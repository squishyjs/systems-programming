// raylib
#include <raylib.h>
#include <raymath.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

bool rectangle_out_bounds(Rectangle rec, int s_width, int s_height) {
    // where the 'x' position represents the top-left corner
    // where the 'y' position represents the top-left corner
    return (rec.x < 0 || rec.x + rec.width >= s_width ||
            rec.y < 0 || rec.y + rec.height >= s_height);
}

void move_rectangle() {
    const int FPS_RATE = 120;
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;
    const char *WINDOW_TITLE = "squarish rectangle";

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS_RATE);

    int rec_height, rec_width;
    rec_width = rec_height = 30;
    Rectangle my_rec = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, rec_width, rec_height};

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_D)) {
            my_rec.x += 2.0f;
        }
        if (IsKeyDown(KEY_A)) {
            my_rec.x -= 2.0f;
        }
        if (IsKeyDown(KEY_S)) {
            my_rec.y += 2.0f;
        }
        if (IsKeyDown(KEY_W)) {
            my_rec.y -= 2.0f;
        }

        // screen collision detection
        if (rectangle_out_bounds(my_rec, SCREEN_WIDTH, SCREEN_HEIGHT)) {
            my_rec.x = Clamp(my_rec.x, my_rec.width, SCREEN_WIDTH - my_rec.width);          // value, min, max
            my_rec.y = Clamp(my_rec.y, my_rec.height, SCREEN_HEIGHT - my_rec.height);       // value, min, max
            printf("Out of bounds!\n");
        }

        BeginDrawing();

            ClearBackground(BLACK);
            // draw rectangle
            DrawRectangleRec(my_rec, MAGENTA);      // render the rectangle behind the text
            const char *description = "Move the square with W-A-S-D!";
            const char font_size = 20;
            DrawText(description, SCREEN_WIDTH / 3.3, 50, font_size, WHITE);

        EndDrawing();
    }

    CloseWindow();
}

void move_circle() {
    const int FPS_RATE = 120;
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;
    const char *WINDOW_TITLE = "FIRST RAYLIB PROGRAM";
    const int radius = 50;
    bool collision = false;


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS_RATE);
    Vector2 ball_position = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};

    // main game loop
    while (!WindowShouldClose()) {      // window close button or ESC
        // update
        // ----------------------------------------------------------
        if (IsKeyDown(KEY_D)) {
            ball_position.x += 2.0f;
        }
        if (IsKeyDown(KEY_A)) {
            ball_position.x -= 2.0f;
        }
        if (IsKeyDown(KEY_S)) {
            ball_position.y += 2.0f;
        }
        if (IsKeyDown(KEY_W)) {
            ball_position.y -= 2.0f;
        }

        // draw
        BeginDrawing();

            ClearBackground(BLACK);

            const char *description = "Move the ball with WASD!";
            const char font_size = 20;
            DrawText(description, SCREEN_WIDTH / 2.9, 50, font_size, WHITE);
            DrawCircleV(ball_position, radius, RED);

        EndDrawing();

    }

    CloseWindow();
}

int main(void) {

    // move_circle();
    move_rectangle();

    return EXIT_SUCCESS;
}
