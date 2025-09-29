#include "entities.hpp"

#include "components.hpp"

void create_world(ecs_s* ecs) {
    auto entity = ecs->entities.create();
    auto world  = ecs->components.add<world_s>(entity);

    world->ecs = ecs;
}

void create_player(ecs_s* ecs, float x, float y) {
    auto entity = ecs->entities.create();
    ecs->components.add<player_s>(entity);
    auto body  = ecs->components.add<body_s>(entity);
    body->type = body_type::circle;
    body->x    = x;
    body->y    = y;
    body->r    = 25.0f;
}