#include "unitypes.h"
#include "dbg.h"
#include <math.h>
#include <raylib.h>

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistdio.h>
#include <stdio.h>
#include <string.h>

#define MAX_BULLETS     1000
#define MAX_ASTEROIDS   1000
#define MAX_BUF         100
#define MAX_TEXTURES    1000
#define FONT_SIZE       20
#define FONT_LIVES_X    25
#define FONT_LIVES_Y    20
#define FONT_SCORE_X    775
#define FONT_SCORE_Y    500

// ------------------------------------------------------------------
// screen consts
// ------------------------------------------------------------------
static const int screen_width = 900;
static const int screen_height = 550;
static const int fps_rate = 120;
static const char *window_title = "asteroids";

static int lives = 3;                               // initial lives count
static int score = 0;                               // initial score
static int num_textures = 0;
static const int bullet_damage = 5;
static const int asteroid_speed = 5;

typedef enum {
    SMALL=1,
    MEDIUM,
    LARGE,
} AsteroidSize;

typedef enum {
    ROCK,
    PLATINUM,
    CARBON,
    DIAMOND
} AsteroidType;

typedef struct {
   Image image;
    Texture2D texture;
    int screen_h;
    int screen_w;
    Color colour;
} GameTexture;

typedef struct Ship {
    GameTexture sprite;
    Vector2 position;
    Vector2 velocity;                               // speed
    float rotation;
    int radius;
    int sides;
    int bullets[MAX_BULLETS];
    Color colour;
} Ship;

typedef struct Asteroid {
    const char *name;
    GameTexture sprite;
    Vector2 velocity;
    float size;
    float rotation;
    AsteroidType a_type;
    AsteroidSize a_size;
} Asteroid;

// -------------------------------------------------------------------
// local module signatures
// -------------------------------------------------------------------
static void update_game(Ship *ship);
static void draw_game(Ship *ship, GameTexture *game_textures);
static void update_draw_game(Ship *ship, GameTexture *game_textures);

// object render functions
void render_text(void);
void render_ship(Ship *ship);
void render_asteroids(Asteroid *asteroids);
void render_game(Ship *ship);
// collision functions
void check_ship_asteroid_collision(void);
void check_bullet_asteroid_collsion(void);
void check_bullet_ship_collsion(void);
void check_ship_screen_collision(void);
void check_asteroid_screen_collision(void);
void check_bullet_ship_collsion(void);
void check_ship_screen_collision(void);
// textures
void load_textures(GameTexture *textures);

void render_text() {
    const char *lives_text = "Lives: ";
    const char *score_text = "Score: ";

    // dynamimc lives counter
    char lives_ctr[MAX_BUF];
    sprintf(lives_ctr, "%d", lives);

    // dynamic score counter
    char score_ctr[MAX_BUF];
    sprintf(score_ctr, "%d", score);

    // TODO: render ship icon lives counter
    DrawText(lives_text, FONT_LIVES_X, FONT_LIVES_Y, FONT_SIZE, MAROON);
    DrawText(lives_ctr, FONT_LIVES_X + 66, FONT_LIVES_Y, FONT_SIZE, MAROON);
    DrawText(score_text, FONT_SCORE_X, FONT_SCORE_Y, FONT_SIZE, YELLOW);
    DrawText(score_ctr, FONT_SCORE_X + 78, FONT_SCORE_Y, FONT_SIZE, YELLOW);
}

void render_ship(Ship *my_ship) {
    // FIXME: change ship so it is rendered using vector lines
    DrawPoly(my_ship->position, my_ship->sides, my_ship->radius, my_ship->rotation, my_ship->colour);
    DrawTexture(my_ship->sprite.texture,
                my_ship->position.x,
                my_ship->position.y,
                my_ship->sprite.colour);
}

void render_asteroids(Asteroid *asteroids) { }

void render_game(Ship *ship) {
    ClearBackground(BLACK);

    render_ship(ship);
    render_text();
    // TODO: render_asteroids();
}

static void update_game(Ship *ship) {
    if (IsKeyDown(KEY_A)) {
        ship->rotation -= 3.0f;
    }
    if (IsKeyDown(KEY_D)) {
        ship->rotation += 3.0f;
    }

    // handle boosting, velocity, friction
    if (IsKeyDown(KEY_W)) {
        float angle_rad = ship->rotation * DEG2RAD;
        Vector2 direction_vector = {
            cosf(angle_rad),
            sinf(angle_rad)
        };

        // boost (accelerate)
        ship->velocity.x += (direction_vector.x * 0.1f);
        ship->velocity.y += (direction_vector.y * 0.1f);
    } else {
        // apply friction
        ship->velocity.x *= 0.99f;
        ship->velocity.y *= 0.99f;
    }

    ship->position.x += ship->velocity.x;
    ship->position.y += ship->velocity.y;
}

static void draw_game(Ship *ship, GameTexture *game_textures) {

    BeginDrawing();
        for (int i = 0; i < num_textures; ++i) {
            DrawTexture(game_textures[i].texture,
                        game_textures[i].screen_w / 2,
                        game_textures[i].screen_h / 2,
                        game_textures[i].colour);
        }

        render_game(ship);

    EndDrawing();

}

static void update_draw_game(Ship *ship, GameTexture *game_textures) {

    // render a single frame
    update_game(ship);
    draw_game(ship, game_textures);

}

Ship init_ship(Vector2 init_pos) {
    Ship space_ship;
    // attributes
    space_ship.position.x = init_pos.x;
    space_ship.position.y = init_pos.y;
    space_ship.radius = 20;
    space_ship.velocity.x = 0;
    space_ship.velocity.y = 0;
    space_ship.rotation = 0;
    space_ship.colour = BLUE;
    space_ship.sides  = 3;

    GameTexture ship_sprite;
    //------------------------------------------------------------
    Image ship_image = LoadImage("./assets/spaceship_normal.png");
    Texture2D space_texture = LoadTextureFromImage(ship_image);
    //------------------------------------------------------------
    space_ship.sprite.image = ship_image;
    space_ship.sprite.texture = space_texture;

    UnloadImage(space_ship.sprite.image);
    num_textures++;

    return space_ship;
}

Asteroid *init_asteroids(int max_asteroids) {
    Asteroid *asteroids = malloc(sizeof(Asteroid) * MAX_ASTEROIDS);
    if (asteroids == NULL) {
        fprintf(stderr, "error: memory alloc failed");
        check_mem(asteroids);
        exit(1);
    }

    // initilaise asteroids
    for (int i = 0; i < max_asteroids; ++i) {
        // TODO: custom init asteroid
        // randomise location, velocity, size and type
        asteroids[i].name = "BIG ROCK";
        asteroids[i].a_size = LARGE;
        asteroids[i].a_type = ROCK;
        asteroids[i].velocity = (Vector2){0.0f, 0.0f};
        asteroids[i].size = 4.0f;
        asteroids[i].rotation = 0.5f;
    }

    return asteroids;

error:
    fprintf(stderr, "error: memory alloc failed");
    return asteroids;
}

// WARNING: NOT NEEDED
void load_textures(GameTexture *textures) {

    // space ship
    Image space_ship = LoadImage("./assets/spaceship_normal.png");
    Texture2D space_texture = LoadTextureFromImage(space_ship);
    textures[0].texture = space_texture;
    textures[0].image = space_ship;
    textures[0].screen_h = screen_height;
    textures[0].screen_w = screen_width;
    textures[0].colour = WHITE;
    UnloadImage(space_ship);
    num_textures++;

    // asteroid 1

}

// program entry point
int main(void) {

    // initalise window and set frame rate
    InitWindow(screen_width, screen_height, window_title);
    SetTargetFPS(fps_rate);

    Vector2 init_pos = {(float)screen_width / 2, (float)screen_height / 2};
    Ship space_ship = init_ship(init_pos);
    Asteroid *asteroids = init_asteroids(MAX_ASTEROIDS);

    // textures
    GameTexture game_textures[MAX_TEXTURES];
    // WARN: NOT NEEDED: load_textures(game_textures);

    // main loop
    while (!WindowShouldClose()) {
        update_draw_game(&space_ship, game_textures);
    }

    // unload textures
    for (int i = 0; i < num_textures; ++i) {
        UnloadTexture(game_textures[i].texture);
    }

    CloseWindow();

    return 0;
}
