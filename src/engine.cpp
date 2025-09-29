#include "engine.hpp"

#include "common.hpp"
#include "components.hpp"
#include "entities.hpp"
#include "neat/ecs.hpp"
#include "raylib.h"
#include "systems.hpp"

engine_s::engine_s() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Ricochet");
    SetTargetFPS(60);
    this->render = LoadRenderTexture(render_width, render_height);

    create_world(&ecs);
    create_player(&ecs, 2.0f, 2.0f);

    // Create box
    create_wall(&ecs, 0.0f, 0.0f, 32.0f, 1.0f);
    create_wall(&ecs, 0.0f, 0.0f, 1.0f, 18.0f);
    create_wall(&ecs, 0.0f, 17.0f, 32.0f, 1.0f);
    create_wall(&ecs, 31.0f, 0.0f, 1.0f, 18.0f);
}

engine_s::~engine_s() {
    UnloadRenderTexture(render);
    CloseWindow();
}

void engine_s::run() {
    while (!WindowShouldClose()) {
        BeginTextureMode(render);
        ClearBackground(RAYWHITE);

        ecs.systems.execute(move_system);
        ecs.systems.execute(draw_system);
        ecs.systems.execute(player_input_system);
        ecs.systems.execute(shoot_system);

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        // Add black bars to keep aspect ratio
        float screen_x            = 0;
        float screen_y            = 0;
        float screen_width        = (float)GetScreenWidth();
        float screen_height       = (float)GetScreenHeight();
        float screen_aspect_ratio = screen_width / screen_height;

        if (screen_aspect_ratio > render_aspect_ratio) {
            // Aspect ratio is wider, limit the screen_width and adjust screen_x
            float new_screen_width = render_aspect_ratio * screen_height;
            screen_x               = (screen_width - new_screen_width) / 2;
            screen_width           = new_screen_width;
        } else if (screen_aspect_ratio < render_aspect_ratio) {
            // Aspect ratio is taller limit the screen_height and adjust screen_y
            float new_screen_height = screen_width / render_aspect_ratio;
            screen_y                = (screen_height - new_screen_height) / 2;
            screen_height           = new_screen_height;
        }

        DrawTexturePro(
            render.texture,
            Rectangle {0, 0, (float)render.texture.width, -(float)render.texture.height},
            Rectangle {screen_x, screen_y, screen_width, screen_height},
            Vector2 {0, 0},
            0.0,
            WHITE);
        EndDrawing();
    }
}