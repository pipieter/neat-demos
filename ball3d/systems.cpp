#include "systems.hpp"

#include <raymath.h>

void physics_system(physics_s* physics) {
    physics->engine.Update(GetFrameTime());
}

void player_input_system(world_s* world) {
    UpdateCamera(&world->camera, CAMERA_THIRD_PERSON);
}

void draw_body(JPH::BodyInterface& interface, body_s* body) {
    auto center    = interface.GetPosition(body->body_id);
    auto transform = MatrixTranslate(center.GetX(), center.GetY(), center.GetZ());
    R3D_DrawMesh(&body->mesh, &body->material, transform);
}

void draw_system(ecs_s& ecs) {
    auto [_1, world]   = ecs.components.first<world_s>();
    auto [_2, physics] = ecs.components.first<physics_s>();
    auto& interface    = physics->engine.Interface();

    R3D_SetBackgroundColor(LIGHTGRAY);
    R3D_Begin(world->camera);

    for (const auto& [body] : ecs.iterate_components<body_s>()) {
        draw_body(interface, body);
    }

    R3D_End();
}
