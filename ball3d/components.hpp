#pragma once

#include <r3d.h>
#include <raylib.h>

#include <neat/ecs.hpp>

struct world_s;
struct body_s;
struct light_s;

using entity_id = neat::ecs::entity_id;
using ecs_s     = neat::ecs::engine<world_s, body_s, light_s>;

struct world_s {
    ecs_s*   ecs;
    Camera3D camera;
};

struct body_s {
    float        cx, cy, cz;
    R3D_Mesh     mesh;
    R3D_Material material;
};

struct light_s {
    R3D_Light light;
};
