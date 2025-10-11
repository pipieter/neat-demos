#include "systems.hpp"

#include <raymath.h>

void physics_system(physics_s* physics) {
    physics->engine.Update(GetFrameTime());
}

void player_input_system(world_s* world) {
    UpdateCamera(&world->camera, CAMERA_THIRD_PERSON);
}

void draw_body(body_s* body, mesh_s* mesh) {
    auto center    = body->interface->GetPosition(body->body_id);
    auto transform = MatrixTranslate(center.GetX(), center.GetY(), center.GetZ());
    R3D_DrawMesh(&mesh->mesh, &mesh->material, transform);
}

void draw_system(ecs_s& ecs) {
    auto [_, world] = ecs.components.first<world_s>();

    R3D_SetBackgroundColor(LIGHTGRAY);
    R3D_Begin(world->camera);

    for (const auto& [body, mesh] : ecs.iterate_components<body_s, mesh_s>()) {
        draw_body(body, mesh);
    }

    R3D_End();
}
