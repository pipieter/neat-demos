#include "systems.hpp"

#include <raylib.h>

#include <cmath>
#include <iostream>

#include "common.hpp"
#include "components.hpp"
#include "entities.hpp"

void draw_b2_body(b2BodyId body_id, b2Vec2 center);

void draw_system(body_s* body) {
}

void draw_aim_system(player_s* player) {
    auto start = world_to_screen_space(player->aim_start);
    auto end   = world_to_screen_space(player->aim_target);
    DrawLineEx({start.x, start.y}, {end.x, end.y}, 3.0, BLUE);
}

void move_system(world_s* world) {
    b2World_Step(world->b2_world, GetFrameTime(), 4);
}

void player_input_system(body_s* body, player_s*) {
    const float velocity = 8.0f;
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

void shoot_system(ecs_s& ecs, body_s* body, player_s* player) {
    auto mouse = GetMousePosition();

    auto center    = body->center();
    auto target    = screen_to_world_space({mouse.x, mouse.y});
    auto direction = b2Normalize(target - center);
    auto offset    = 0.75 * direction;

    player->aim_start  = center + offset;
    player->aim_target = target;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        auto direction = b2Normalize(player->aim_target - player->aim_start);
        create_bullet(&ecs, player->aim_start.x, player->aim_start.y, direction.x, direction.y);
    }
}

entity_id get_entity_from_box2d_shape(ecs_s& ecs, b2ShapeId shape_id) {
    b2BodyId body_id = b2Shape_GetBody(shape_id);
    for (const auto& [entity, body] : ecs.iterate<body_s>()) {
        if (B2_ID_EQUALS(body_id, body->b2_id)) {
            return entity;
        }
    }
    return neat::ecs::invalid_entity;
}

void bullet_enemy_collision_system(ecs_s& ecs, world_s* world) {
    auto contacts = b2World_GetContactEvents(world->b2_world);

    std::vector<entity_id> enemies_to_delete;

    for (int i = 0; i < contacts.beginCount; i++) {
        auto contact = contacts.beginEvents[i];
        auto a       = get_entity_from_box2d_shape(ecs, contact.shapeIdA);
        auto b       = get_entity_from_box2d_shape(ecs, contact.shapeIdB);

        auto enemy = neat::ecs::invalid_entity;

        // Check if bullet a hits enemy b
        if ((ecs.components.has<bullet_s>(a) && ecs.components.has<enemy_s>(b))) {
            enemy = b;
        }
        // Check if bullet a hits enemy b
        else if ((ecs.components.has<bullet_s>(b) && ecs.components.has<enemy_s>(a))) {
            enemy = a;
        }

        if (enemy != neat::ecs::invalid_entity) {
            enemies_to_delete.push_back(enemy);
        }
    }

    for (auto enemy : enemies_to_delete) {
        remove_entity(&ecs, enemy);
    }
}

void draw_b2_body(b2BodyId body_id, b2Vec2 center) {
    const float            line_thickness = 5.0;
    const float            scale          = render_scale;
    int                    shape_count    = b2Body_GetShapeCount(body_id);
    std::vector<b2ShapeId> shape_ids(shape_count);
    b2Body_GetShapes(body_id, shape_ids.data(), shape_ids.size());

    for (auto shape_id : shape_ids) {
        b2ShapeType type = b2Shape_GetType(shape_id);

        if (type == b2_circleShape) {
            b2Circle circle = b2Shape_GetCircle(shape_id);
            float    x      = scale * center.x;
            float    y      = scale * center.y;
            float    r      = scale * circle.radius;
            DrawRing({x, y}, r - line_thickness, r, 0, 360, 32, GREEN);
        } else if (type == b2_polygonShape) {
            // Should be a rectangle
            // The first vertex is always the half-width and half-height of the rectangle
            b2Polygon polygon = b2Shape_GetPolygon(shape_id);
            float     w       = scale * std::abs(2.0 * polygon.vertices[0].x);
            float     h       = scale * std::abs(2.0 * polygon.vertices[0].y);
            float     x       = scale * center.x - w / 2;
            float     y       = scale * center.y - h / 2;
            DrawRectangleLinesEx({x, y, w, h}, line_thickness, RED);
        } else {
            std::cout << "Unsupported shape type " << type << std::endl;
        }
    }
}

void debug_system(world_s* world) {
    if (IsKeyPressed(KEY_GRAVE)) {
        world->debug = !world->debug;
    }
}

void debug_draw_system(ecs_s& ecs, world_s* world) {
    if (!world->debug) {
        return;
    }

    for (const auto& [body] : ecs.iterate_components<body_s>()) {
        draw_b2_body(body->b2_id, body->center());
    }
}