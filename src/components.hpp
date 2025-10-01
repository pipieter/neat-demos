#pragma once

#include <box2d/box2d.h>

#include <neat/ecs.hpp>

struct world_s;
struct body_s;
struct player_s;
struct blocking_s;

using entity_id = neat::ecs::entity_id;
using ecs_s     = neat::ecs::engine<
    world_s,
    body_s,
    player_s,
    blocking_s>;

struct world_s {
    ecs_s*    ecs;
    b2WorldId b2_world;
};

enum class body_type {
    rectangle = 0,
    circle    = 1
};

struct body_s {
    body_type type;
    b2BodyId  b2_id;

    b2Vec2 center() const;
};

struct player_s {
    b2Vec2 aim_start;
    b2Vec2 aim_target;
};

struct blocking_s {};