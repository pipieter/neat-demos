#pragma once

#include <neat/ecs.hpp>

struct world_s;
struct transform_s;
struct hitbox_s;
struct move_s;
struct player_s;

using entity_id = neat::ecs::entity_id;
using ecs_st    = neat::ecs::engine<
    world_s,
    transform_s,
    hitbox_s,
    move_s,
    player_s>;

struct world_s {
    ecs_st* ecs;
    struct {
        float w, h;
    } level;
};

struct transform_s {
    float x, y;
};

enum class hitbox_type {
    rectangle = 0,
    circle    = 1
};

struct hitbox_s {
    hitbox_type type;
    union {
        float radius;
        struct {
            float w, h;
        };
    };
};

struct move_s {
    float dx, dy;
};

struct player_s {};