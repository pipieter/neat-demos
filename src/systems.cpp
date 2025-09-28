#include "systems.hpp"

#include <cmath>

#include "components.hpp"
#include "raylib.h"

void draw_system(transform_s* transform) {
    DrawRectangle((int)transform->x, (int)transform->y, 20, 20, RED);
}

void move_system(ecs_st& ecs) {
    for (auto [entity, transform, move] : ecs.iterate<transform_s, move_s>()) {
        transform->x += move->dx;
        transform->y += move->dy;
        ecs.components.remove<move_s>(entity);
    }
}

void player_input_system(ecs_st& ecs, entity_id entity, player_s*) {
    const float velocity = 300.0f;
    float       dx       = 0;
    float       dy       = 0;

    if (IsKeyDown(KEY_W))
        dy -= velocity;
    if (IsKeyDown(KEY_A))
        dx -= velocity;
    if (IsKeyDown(KEY_S))
        dy += velocity;
    if (IsKeyDown(KEY_D))
        dx += velocity;

    dx = dx * GetFrameTime();
    dy = dy * GetFrameTime();
    ecs.components.add<move_s>(entity, dx, dy);
}