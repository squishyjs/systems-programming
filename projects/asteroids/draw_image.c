#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistdio.h>
#include <unistd.h>
#include <stdbool.h>

const int screen_width = 800;
const int screen_height = 450;
const char *title = "space ship";

int main(void) {
    InitWindow(screen_width, screen_height, title);

    Image space_ship = LoadImage("./assets/spaceship_normal.png");
    Texture2D space_texture = LoadTextureFromImage(space_ship);
    UnloadImage(space_ship);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(space_texture, screen_width / 2, screen_height / 2, WHITE);
        EndDrawing();
    }

    // de-initiliazation
    UnloadTexture(space_texture);

    CloseWindow();

    return EXIT_SUCCESS;
}
