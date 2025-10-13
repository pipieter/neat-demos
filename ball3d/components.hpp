#pragma once

#include <r3d.h>
#include <raylib.h>

#include <neat/ecs.hpp>

#include "physics.hpp"

struct world_s;
struct physics_s;
struct body_s;
struct mesh_s;
struct ball_s;
struct rotation_s;
struct light_s;

using entity_id = neat::ecs::entity_id;
using ecs_s     = neat::ecs::engine<world_s, physics_s, body_s, mesh_s, ball_s, rotation_s, light_s>;

struct world_s {
    ecs_s*   ecs;
    Camera3D camera;
    float    position;
};

struct physics_s {
    PhysicsEngine engine;
};

struct body_s {
    // TODO this needs to be manually deleted from the physics engine
    JPH::BodyInterface* interface;
    JPH::BodyID         id;
};

struct mesh_s {
    R3D_Mesh     mesh;
    R3D_Material material;
};

struct ball_s {
};

struct rotation_s {
    float angle_x {0};
    float angle_z {0};
};

struct light_s {
    R3D_Light light;
};
