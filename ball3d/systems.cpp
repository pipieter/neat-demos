#include "systems.hpp"

#include <raymath.h>

void physics_system(ecs_s& ecs) {}

void player_input_system(world_s* world) {
    UpdateCamera(&world->camera, CAMERA_THIRD_PERSON);
}

void draw_body(body_s& body) {
    auto transform = MatrixTranslate(body.cx, body.cy, body.cz);
    R3D_DrawMesh(&body.mesh, &body.material, transform);
}

void draw_system(ecs_s& ecs) {
    auto [_, world] = ecs.components.first<world_s>();

    R3D_SetBackgroundColor(LIGHTGRAY);
    R3D_Begin(world->camera);

    for (const auto& [body] : ecs.iterate_components<body_s>()) {
        draw_body(*body);
    }

    R3D_End();
}
