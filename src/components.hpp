#pragma once

#include <box2d/box2d.h>
#include <raylib.h>

#include <neat/ecs.hpp>

#include "assets.hpp"

struct world_s;
struct body_s;
struct player_s;
struct bullet_s;
struct enemy_s;
struct blocking_s;
struct sprite_s;
struct marked_for_deletion_s;

using entity_id = neat::ecs::entity_id;
using ecs_s     = neat::ecs::engine<
    world_s,
    body_s,
    player_s,
    bullet_s,
    enemy_s,
    blocking_s,
    sprite_s,
    marked_for_deletion_s>;

struct world_s {
    bool      debug;
    ecs_s*    ecs;
    b2WorldId b2_world;
    struct {
        Texture2D image;
    } level;
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

struct bullet_s {
    int remaining_ricochets;
};

struct enemy_s {};

struct blocking_s {};

struct sprite_s {
    Sprite2D sprite;
    float    pixels_per_unit {8};
    bool     flip_h {false};
    bool     flip_v {false};
};

struct marked_for_deletion_s {};