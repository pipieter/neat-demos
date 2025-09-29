#include "systems.hpp"

#include <raylib.h>

#include <cmath>
#include <iostream>

#include "components.hpp"

void draw_system(body_s* body) {
    const float scale    = 100.0f;
    auto        position = b2Body_GetPosition(body->b2_id);

    int                    shape_count = b2Body_GetShapeCount(body->b2_id);
    std::vector<b2ShapeId> shape_ids(shape_count);
    b2Body_GetShapes(body->b2_id, shape_ids.data(), shape_ids.size());

    for (auto shape_id : shape_ids) {
        b2ShapeType type = b2Shape_GetType(shape_id);

        if (type == b2_circleShape) {
            b2Circle circle = b2Shape_GetCircle(shape_id);
            float    x      = scale * position.x;
            float    y      = scale * position.y;
            float    r      = scale * circle.radius;
            DrawCircle(x, y, r, GREEN);
        } else if (type == b2_polygonShape) {
            b2Polygon polygon = b2Shape_GetPolygon(shape_id);
            float     w       = std::abs(2.0 * scale * polygon.vertices[0].x);
            float     h       = std::abs(2.0 * scale * polygon.vertices[0].y);
            float     x       = scale * position.x - w / 2;
            float     y       = scale * position.y - h / 2;
            DrawRectangle(x, y, w, h, RED);
        } else {
            std::cout << "Unsupported shape type " << type << std::endl;
        }
    }
}

void move_system(world_s* world) {
    b2World_Step(world->b2_world, GetFrameTime(), 4);
}

void player_input_system(body_s* body, player_s*) {
    const float velocity = 3.0f;
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