#include "systems.hpp"

void physics_system(ecs_s& ecs) {}

void player_input_system(world_s* world) {
    float speed = 1.0f * GetFrameTime();

    if (IsKeyDown(KEY_W))
        world->camera.position.x -= speed;
    if (IsKeyDown(KEY_S))
        world->camera.position.x += speed;
    if (IsKeyDown(KEY_A))
        world->camera.position.z -= speed;
    if (IsKeyDown(KEY_D))
        world->camera.position.z += speed;
}

void update_light(const Shader& shader, const light_s& light) {}

void light_system(ecs_s& ecs, world_s* world) {
    float cameraPos[3] = {world->camera.position.x, world->camera.position.y, world->camera.position.z};
    SetShaderValue(world->shader, world->shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    for (const auto& [light] : ecs.iterate_components<light_s>()) {
        // Send to shader light enabled state and type
        SetShaderValue(world->shader, light->enabled_loc, &light->enabled, SHADER_UNIFORM_INT);
        SetShaderValue(world->shader, light->type_loc, &light->type, SHADER_UNIFORM_INT);

        // Send to shader light position values
        float position[3] = {light->position.x, light->position.y, light->position.z};
        SetShaderValue(world->shader, light->position_loc, position, SHADER_UNIFORM_VEC3);

        // Send to shader light target position values
        float target[3] = {light->target.x, light->target.y, light->target.z};
        SetShaderValue(world->shader, light->target_loc, target, SHADER_UNIFORM_VEC3);

        // Send to shader light color values
        float color[4] = {(float)light->color.r / (float)255, (float)light->color.g / (float)255,
                          (float)light->color.b / (float)255, (float)light->color.a / (float)255};
        SetShaderValue(world->shader, light->color_loc, color, SHADER_UNIFORM_VEC4);
    }
}

void draw_body(body_s& body) {
    if (body.type == body_type::sphere) {
        DrawSphere({body.sphere.x, body.sphere.y, body.sphere.z}, body.sphere.r, RED);
    } else if (body.type == body_type::box) {
        DrawCube({body.box.x, body.box.y, body.box.z}, body.box.w, body.box.h, body.box.l, GRAY);
    }
}

void draw_system(ecs_s& ecs) {
    auto [_, world] = ecs.components.first<world_s>();

    BeginMode3D(world->camera);
    BeginShaderMode(world->shader);

    for (const auto& [body] : ecs.iterate_components<body_s>()) {
        draw_body(*body);
    }

    EndShaderMode();
    EndMode3D();
}
