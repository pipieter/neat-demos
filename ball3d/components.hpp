#pragma once

#include <raylib.h>

#include <neat/ecs.hpp>

struct world_s;
struct body_s;
struct material_s;
struct light_s;

using entity_id = neat::ecs::entity_id;
using ecs_s     = neat::ecs::engine<world_s, body_s, material_s, light_s>;

struct world_s {
    ecs_s*   ecs;
    Shader   shader;
    Camera3D camera;
};

enum class body_type : unsigned char {
    sphere = 1,
    box    = 2,
};

struct body_s {
    body_type type;
    union {
        struct {
            float x, y, z;
            float r;
        } sphere;
        struct {
            float x, y, z;
            float w, h, l;
        } box;
    };
};

struct material_s {};

enum light_type : unsigned char {
    directional = 0,
    point       = 1
};

struct light_s {
    int     type;
    bool    enabled;
    Vector3 position;
    Vector3 target;
    Color   color;
    float   attenuation;

    // Shader locations
    int enabled_loc;
    int type_loc;
    int position_loc;
    int target_loc;
    int color_loc;
    int attenuation_loc;
};
