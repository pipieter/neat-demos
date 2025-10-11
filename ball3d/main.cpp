#include <raylib.h>

#include "components.hpp"
#include "entities.hpp"
#include "systems.hpp"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Ball 3D");

    ecs_s ecs;
    auto  world_id = create_world(ecs);
    auto  world    = ecs.components.get<world_s>(world_id);

    (void)create_ball(ecs, 1.0, 1.0, 3.0, 1.0);
    (void)create_maze(ecs, 5.0, -1.0, 0.0, 10.0, 1.0, 10.0);
    (void)create_light(ecs, world->shader, 0.0, 5.0, 0.0);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLUE);

        ecs.systems.execute(physics_system);
        ecs.systems.execute(player_input_system);
        ecs.systems.execute(light_system);
        ecs.systems.execute(draw_system);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}