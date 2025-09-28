#include "entities.hpp"

#include "box2d/box2d.h"
#include "box2d/types.h"

void create_world(ecs_s* ecs) {
    auto entity = ecs->entities.create();
    auto world  = ecs->components.add<world_s>(entity);

    world->ecs = ecs;

    // TODO look into https://box2d.org/documentation/md_simulation.html
    auto b2_world_def      = b2DefaultWorldDef();
    b2_world_def.gravity.x = 0.0f;
    b2_world_def.gravity.y = 0.0f;
    world->b2_world        = b2CreateWorld(&b2_world_def);
}

void create_player(ecs_s* ecs, float x, float y) {
    auto [_, world] = ecs->components.first<world_s>();

    auto entity = ecs->entities.create();
    ecs->components.add<player_s>(entity);

    b2BodyDef body_def  = b2DefaultBodyDef();
    body_def.position.x = x;
    body_def.position.y = y;
    body_def.type       = b2_kinematicBody;

    body_s* body = ecs->components.add<body_s>(entity);
    body->b2_id  = b2CreateBody(world->b2_world, &body_def);

    b2Polygon  box       = b2MakeBox(1.0f, 1.0f);
    b2ShapeDef shape_def = b2DefaultShapeDef();
    b2CreatePolygonShape(body->b2_id, &shape_def, &box);
}