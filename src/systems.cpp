#include "systems.hpp"

#include <raylib.h>

#include <cmath>
#include <iostream>

#include "box2d/box2d.h"
#include "components.hpp"

void draw_system(body_s* body) {
    auto position = b2Body_GetPosition(body->b2_id);
    float x = position.x * 300.0f;
    float y = position.y * 300.0f;
    DrawRectangle((int)x, (int)y, 20.0f, 20.0f, RED);
}

void move_system(ecs_s& ecs) {
    auto [_, world] = ecs.components.first<world_s>();
    b2World_Step(world->b2_world, GetFrameTime(), 4);
}

void player_input_system(body_s* body) {
    const float velocity = 1.0f;
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

    b2Body_SetLinearVelocity(body->b2_id, {dx, dy});
}