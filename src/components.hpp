#pragma once

#include <neat/ecs.hpp>

struct world_s;
struct body_s;
struct velocity_s;
struct player_s;
struct blocking_s;

using entity_id = neat::ecs::entity_id;
using ecs_s     = neat::ecs::engine<
    world_s,
    body_s,
    velocity_s,
    player_s,
    blocking_s>;

struct world_s {
    ecs_s* ecs;
    struct {
        float w, h;
    } level;
};

enum class body_type {
    rectangle = 0,
    circle    = 1
};

struct body_s {
    body_type type;
    float     x, y;
    union {
        struct {
            float w, h;
        };
        struct {
            float r;
        };
    };
};

struct velocity_s {
    float dx, dy;
};

struct player_s {};

struct blocking_s {};