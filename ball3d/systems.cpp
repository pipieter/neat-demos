#include "systems.hpp"

#include <raymath.h>

void physics_system(physics_s* physics) {
    physics->engine.Update(GetFrameTime());
}

void player_input_system(world_s* world) {
    UpdateCamera(&world->camera, CAMERA_THIRD_PERSON);
}

void draw_body(body_s* body, mesh_s* mesh) {
    auto position = body->interface->GetPosition(body->id);
    auto rotation = body->interface->GetRotation(body->id);

    JPH::RMat44 jolt_transform = JPH::RMat44::sRotationTranslation(rotation, position);
    JPH::Float4 vectors[4];
    jolt_transform.StoreFloat4x4(vectors);
    Matrix transform;
    transform.m0  = vectors[0].x;
    transform.m1  = vectors[0].y;
    transform.m2  = vectors[0].z;
    transform.m3  = vectors[0].w;
    transform.m4  = vectors[1].x;
    transform.m5  = vectors[1].y;
    transform.m6  = vectors[1].z;
    transform.m7  = vectors[1].w;
    transform.m8  = vectors[2].x;
    transform.m9  = vectors[2].y;
    transform.m10 = vectors[2].z;
    transform.m11 = vectors[2].w;
    transform.m12 = vectors[3].x;
    transform.m13 = vectors[3].y;
    transform.m14 = vectors[3].z;
    transform.m15 = vectors[3].w;

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

void debug_ball_respawn(body_s* body, ball_s*) {
    if (IsKeyPressed(KEY_R)) {
        body->interface->SetPosition(body->id, {0, 10, 0}, JPH::EActivation::Activate);
        body->interface->SetLinearVelocity(body->id, {0, 0, 0});
    }
}

void debug_rotation_system(body_s* body, rotation_s*) {
    float amount_y = 0.50 * GetFrameTime();
    float amount_z = 0.25 * std::sin(GetTime() / 10);

    auto old_rotation = body->interface->GetRotation(body->id);
    auto rotation_y   = JPH::Quat::sRotation({0, 1, 0}, old_rotation.GetRotationAngle({0, 1, 0}) + amount_y);
    auto rotation_z   = JPH::Quat::sRotation({0, 0, 1}, amount_z);
    auto new_rotation = rotation_y * rotation_z;

    body->interface->SetRotation(body->id, new_rotation, JPH::EActivation::Activate);
}
