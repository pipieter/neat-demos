#include "engine.hpp"

#include "components.hpp"
#include "entities.hpp"
#include "neat/ecs.hpp"
#include "raylib.h"
#include "systems.hpp"

engine_s::engine_s() {
    InitWindow(1280, 720, "Ricochet");
    SetTargetFPS(60);

    create_world(&ecs);
    create_player(&ecs, 2.0f, 2.0f);

    // Create box
    create_wall(&ecs, 0.0f, 0.0f, 32.0f, 1.0f);
    create_wall(&ecs, 0.0f, 0.0f, 1.0f, 18.0f);
    create_wall(&ecs, 0.0f, 17.0f, 32.0f, 1.0f);
    create_wall(&ecs, 31.0f, 0.0f, 1.0f, 18.0f);
}

engine_s::~engine_s() {
    CloseWindow();
}

void engine_s::run() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        ecs.systems.execute(move_system);
        ecs.systems.execute(draw_system);
        ecs.systems.execute(player_input_system);
        ecs.systems.execute(shoot_system);

        EndDrawing();
    }
}