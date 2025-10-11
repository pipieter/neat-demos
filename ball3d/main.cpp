#include <r3d.h>
#include <raylib.h>

#include "components.hpp"
#include "entities.hpp"
#include "systems.hpp"

int main() {
    JPH::RegisterDefaultAllocator();
    JPH::Trace              = TraceImpl;
    JPH::AssertFailed       = AssertFailedImpl;
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    InitWindow(1280, 720, "Ball 3D");
    R3D_Init(1280, 720, 0);

    ecs_s ecs;
    (void)create_world(ecs);
    (void)create_ball(ecs, 0.0, 10.0, 0.0, 1.0);
    (void)create_maze(ecs, 0.0, -1.0, 0.0, 3.0, 1.0, 10.0);
    (void)create_light(ecs, 0.0, 5.0, 0.0);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLUE);

        ecs.systems.execute(physics_system);
        ecs.systems.execute(player_input_system);
        ecs.systems.execute(draw_system);

        EndDrawing();
    }

    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;

    CloseWindow();

    return 0;
}