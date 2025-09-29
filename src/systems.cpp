#include "systems.hpp"

#include <raylib.h>

#include <cmath>
#include <iostream>

#include "components.hpp"

void draw_system(body_s* body) {
    if (body->type == body_type::rectangle) {
        DrawRectangle(body->x, body->y, body->w, body->h, RED);
    } else if (body->type == body_type::circle) {
        DrawCircle(body->x, body->y, body->r, GREEN);
    }
}

void move_system(body_s* body, velocity_s* velocity) {
    body->x += velocity->dx * GetFrameTime();
    body->y += velocity->dy * GetFrameTime();
}

void player_input_system(ecs_s& ecs, entity_id entity, player_s*) {
    const float velocity = 400.0f;
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

    ecs.components.add<velocity_s>(entity, dx, dy);
}