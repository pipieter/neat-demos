#include <r3d.h>
#include <raylib.h>

#include "components.hpp"
#include "entities.hpp"
#include "systems.hpp"

int main() {
    std::cout << "Starting program." << std::endl;

    JPH::RegisterDefaultAllocator();
    JPH::Trace              = TraceImpl;
    JPH::AssertFailed       = AssertFailedImpl;
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Ball 3D");
    SetTargetFPS(60);
    R3D_Init(1920, 1080, R3D_FLAG_FXAA);

    ecs_s ecs;
    (void)create_world(ecs);
    (void)create_ball(ecs, 0.0, 3.0, 0.0, 1.0);
    (void)create_box(ecs, 0.0, -1.0, 0.0, 5.0, 1.5, 10.0, 0.2);
    (void)create_light(ecs, 0.0, 5.0, 0.0);

    auto [_, physics] = ecs.components.first<physics_s>();
    physics->engine.Ready();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLUE);

        ecs.systems.execute(log_enable_system);
        ecs.systems.execute(physics_system);
        ecs.systems.execute(ball_reset_system);
        ecs.systems.execute(platform_rotation_system);
        ecs.systems.execute(draw_system);

        EndDrawing();
    }

    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;

    CloseWindow();

    std::cout << "Ending program." << std::endl;

    return 0;
}