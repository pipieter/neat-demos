#include "entities.hpp"

#include <box2d/box2d.h>

#include "assets.hpp"
#include "components.hpp"

void remove_entity(ecs_s* ecs, entity_id entity) {
    // The body needs to be manually removed from the bxo2d world, otherwise it will persist
    if (ecs->components.has<body_s>(entity)) {
        b2DestroyBody(ecs->components.get<body_s>(entity)->b2_id);
    }

    ecs->entities.remove(entity);
}

entity_id create_world(ecs_s* ecs) {
    auto entity = ecs->entities.create();
    auto world  = ecs->components.add<world_s>(entity);

    world->ecs = ecs;

    // Create physics world
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity.x  = 0;
    world_def.gravity.y  = 0;

    world->b2_world    = b2CreateWorld(&world_def);
    world->debug       = true;
    world->level.image = assets::level01_image;
    return entity;
}

entity_id create_player(ecs_s* ecs, float x, float y) {
    auto [_, world] = ecs->components.first<world_s>();
    auto entity     = ecs->entities.create();

    // Create player
    ecs->components.add<player_s>(entity);

    // Create sprite
    auto sprite    = ecs->components.add<sprite_s>(entity);
    sprite->sprite = assets::player_sprite;

    // Create body
    auto body  = ecs->components.add<body_s>(entity);
    body->type = body_type::circle;

    b2BodyDef body_def  = b2DefaultBodyDef();
    body_def.type       = b2_dynamicBody;
    body_def.position.x = x;
    body_def.position.y = y;
    body->b2_id         = b2CreateBody(world->b2_world, &body_def);

    b2ShapeDef shape_def = b2DefaultShapeDef();
    b2Circle   circle;
    circle.radius   = 0.5f;
    circle.center.x = 0;
    circle.center.y = 0;

    shape_def.filter.categoryBits = 0x0001;
    shape_def.filter.maskBits     = 0xFFFF;
    shape_def.enableContactEvents = true;

    b2CreateCircleShape(body->b2_id, &shape_def, &circle);
    return entity;
}

entity_id create_wall(ecs_s* ecs, float x, float y, float w, float h) {
    auto [_, world] = ecs->components.first<world_s>();
    auto entity     = ecs->entities.create();

    (void)ecs->components.add<blocking_s>(entity);

    auto body  = ecs->components.add<body_s>(entity);
    body->type = body_type::rectangle;

    b2BodyDef body_def  = b2DefaultBodyDef();
    body_def.type       = b2_staticBody;
    body_def.position.x = x + w / 2;  // box2d uses center coordinates
    body_def.position.y = y + h / 2;
    body->b2_id         = b2CreateBody(world->b2_world, &body_def);

    b2ShapeDef shape_def        = b2DefaultShapeDef();
    b2Polygon  box              = b2MakeBox(w / 2, h / 2);
    shape_def.material.friction = 0.0f;

    shape_def.filter.categoryBits = 0x0001;
    shape_def.filter.maskBits     = 0xFFFF;
    shape_def.enableContactEvents = true;

    b2CreatePolygonShape(body->b2_id, &shape_def, &box);
    return entity;
}

entity_id create_bullet(ecs_s* ecs, float x, float y, float dx, float dy) {
    auto [_, world] = ecs->components.first<world_s>();
    auto entity     = ecs->entities.create();

    // Create bullet
    auto bullet                 = ecs->components.add<bullet_s>(entity);
    bullet->remaining_ricochets = 2;

    // Create sprite
    auto sprite    = ecs->components.add<sprite_s>(entity);
    sprite->sprite = assets::bullet_sprite;

    // Create body
    auto body  = ecs->components.add<body_s>(entity);
    body->type = body_type::circle;

    b2BodyDef body_def  = b2DefaultBodyDef();
    body_def.type       = b2_dynamicBody;
    body_def.position.x = x;
    body_def.position.y = y;
    body->b2_id         = b2CreateBody(world->b2_world, &body_def);

    b2ShapeDef shape_def = b2DefaultShapeDef();
    b2Circle   circle;
    circle.radius                  = 0.2f;
    circle.center.x                = 0;
    circle.center.y                = 0;
    shape_def.material.restitution = 1.0f;

    shape_def.filter.categoryBits = 0x0001;
    shape_def.filter.maskBits     = 0xFFFF;
    shape_def.enableContactEvents = true;

    b2CreateCircleShape(body->b2_id, &shape_def, &circle);

    b2Body_ApplyLinearImpulse(body->b2_id, {dx, dy}, {x, y}, true);
    return entity;
}

entity_id create_enemy(ecs_s* ecs, float x, float y) {
    auto entity = create_wall(ecs, x, y, 1, 1);
    ecs->components.add<enemy_s>(entity);
    ecs->components.remove<blocking_s>(entity);

    // Create sprite
    auto sprite    = ecs->components.add<sprite_s>(entity);
    sprite->sprite = assets::enemy_sprite;

    return entity;
}