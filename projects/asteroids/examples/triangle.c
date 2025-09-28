#include "raylib.h"
#include "raymath.h"   // for Vector2Rotate, Vector2* helpers

typedef struct {
    Vector2 pos;        // world position (pivot)
    float   angle;      // radians
    Vector2 vel;        // px/sec
    float   angVel;     // rad/sec
    Vector2 local[3];   // vertices in local space, centered around (0,0)
    float   thickness;
    Color   color;
} Triangle2D;

static void TriGetWorldVerts(const Triangle2D *t, Vector2 out[3]) {
    for (int i = 0; i < 3; ++i) {
        Vector2 rotated = Vector2Rotate(t->local[i], t->angle);
        out[i] = Vector2Add(t->pos, rotated);
    }
}

static void TriDrawWire(const Triangle2D *t) {
    Vector2 v[3];
    TriGetWorldVerts(t, v);
    DrawLineEx(v[0], v[1], t->thickness, t->color);
    DrawLineEx(v[1], v[2], t->thickness, t->color);
    DrawLineEx(v[2], v[0], t->thickness, t->color);
}

static void WrapAround(Vector2 *p, int w, int h) {
    if (p->x < 0) p->x += w; else if (p->x >= w) p->x -= w;
    if (p->y < 0) p->y += h; else if (p->y >= h) p->y -= h;
}

static void TriUpdate(Triangle2D *t, float dt, int screenW, int screenH) {
    t->pos = Vector2Add(t->pos, Vector2Scale(t->vel, dt));
    t->angle += t->angVel * dt;
    WrapAround(&t->pos, screenW, screenH);
}

int main(void) {
    const int W = 900, H = 600;
    InitWindow(W, H, "floating triangle and rocks");
    SetTargetFPS(60);

    // Define a triangle in local space, centered at origin for clean rotation
    // (equilateral-ish; tweak to taste)
    Triangle2D tri = {
        .pos = { W/2.0f, H/2.0f },
        .angle = 0.0f,
        .vel = { 120.0f, -40.0f },     // px/sec
        .angVel = 0.9f,                // rad/sec
        .local = {
            {  0.0f, -60.0f },         // top
            { 50.0f,  35.0f },         // bottom-right
            {-50.0f,  35.0f }          // bottom-left
        },
        .thickness = 1.2f,
        .color = WHITE
    };

    // Simple input tuning
    const float accel = 300.0f;    // px/s^2
    const float drag  = 0.98f;     // velocity damping per frame
    const float spinAccel = 3.0f;  // rad/s^2

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // --- Controls (WASD to accelerate, Q/E to spin) ---
        if (IsKeyDown(KEY_W)) tri.vel.y -= accel * dt;
        if (IsKeyDown(KEY_S)) tri.vel.y += accel * dt;
        if (IsKeyDown(KEY_A)) tri.vel.x -= accel * dt;
        if (IsKeyDown(KEY_D)) tri.vel.x += accel * dt;
        if (IsKeyDown(KEY_Q)) tri.angVel -= spinAccel * dt;
        if (IsKeyDown(KEY_E)) tri.angVel += spinAccel * dt;

        // stop motion
        if (IsKeyPressed(KEY_SPACE)) {
            tri.vel = (Vector2){0}; tri.angVel = 0.0f;
        }

        // reset triangle pos
        if (IsKeyPressed(KEY_R)) {
            tri.pos = (Vector2){W/2.0f, H/2.0f}; tri.angle = 0.0f;
        }

        // light damping so it coasts nicely
        tri.vel = Vector2Scale(tri.vel, powf(drag, GetFPS() > 0 ? 1.0f : 0.0f));
        tri.angVel *= drag;

        // Update physics + wrapping
        TriUpdate(&tri, dt, W, H);

        // --- Render ---
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("WASD move, Q/E spin, SPACE stop, R reset", 16, 16, 20, DARKGRAY);
        TriDrawWire(&tri);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
