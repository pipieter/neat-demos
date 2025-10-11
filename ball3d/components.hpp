#pragma once

#include <r3d.h>
#include <raylib.h>

#include <neat/ecs.hpp>

#include "physics.hpp"

struct world_s;
struct physics_s;
struct body_s;
struct light_s;

using entity_id = neat::ecs::entity_id;
using ecs_s     = neat::ecs::engine<world_s, physics_s, body_s, light_s>;

struct world_s {
    ecs_s*   ecs;
    Camera3D camera;
};

struct physics_s {
   PhysicsEngine engine;
};

struct body_s {
    R3D_Mesh     mesh;
    R3D_Material material;
    JPH::BodyID  body_id;
};

struct light_s {
    R3D_Light light;
};
