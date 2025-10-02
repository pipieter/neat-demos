#include "engine.hpp"

#include "common.hpp"
#include "components.hpp"
#include "entities.hpp"
#include "neat/ecs.hpp"
#include "raylib.h"
#include "systems.hpp"

engine_s::engine_s() {
    this->render = LoadRenderTexture(render_width, render_height);

    create_world(&ecs);
    create_player(&ecs, 2.0f, 2.0f);

    // Create box
    create_wall(&ecs, 0.0f, 0.0f, 32.0f, 1.0f);
    create_wall(&ecs, 0.0f, 0.0f, 1.0f, 18.0f);
    create_wall(&ecs, 0.0f, 17.0f, 32.0f, 1.0f);
    create_wall(&ecs, 31.0f, 0.0f, 1.0f, 18.0f);
    create_wall(&ecs, 1.0f, 12.0f, 30.0f, 1.0f);

    create_enemy(&ecs, 10.0, 10.0);
}

engine_s::~engine_s() {
    UnloadRenderTexture(render);
}

void engine_s::run() {
    while (!WindowShouldClose()) {
        BeginTextureMode(render);
        ClearBackground(RAYWHITE);

        ecs.systems.execute(debug_system);
        ecs.systems.execute(physics_system);
        ecs.systems.execute(bullet_enemy_collision_system);
        ecs.systems.execute(player_input_system);
        ecs.systems.execute(player_aim_flip_system);
        ecs.systems.execute(shoot_system);
        ecs.systems.execute(draw_system);
        ecs.systems.execute(draw_aim_system);
        ecs.systems.execute(debug_draw_system);

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        // Add black bars to keep aspect ratio
        Rectangle screen = get_screen_bounds();

        DrawTexturePro(
            render.texture,
            Rectangle {0, 0, (float)render.texture.width, -(float)render.texture.height},
            screen,
            Vector2 {0, 0},
            0.0,
            WHITE);
        EndDrawing();
    }
}