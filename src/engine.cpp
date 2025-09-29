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
    create_player(&ecs, 100.0f, 100.0f);
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

        EndDrawing();
    }
}