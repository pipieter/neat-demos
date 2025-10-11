#include "entities.hpp"

#include <stdexcept>

#include "assets.hpp"

float world_ambient[4] = {0.4f, 0.4f, 0.4f, 1.0f};

entity_id create_world(ecs_s& ecs) {
    entity_id entity = ecs.entities.create();

    world_s* world = ecs.components.add<world_s>(entity);
    world->ecs     = &ecs;

    // Create camera
    world->camera.up.x       = 0;
    world->camera.up.y       = 1;
    world->camera.up.z       = 0;
    world->camera.fovy       = 45.0f;
    world->camera.position.x = 10;
    world->camera.position.y = 0;
    world->camera.position.z = 0;
    world->camera.projection = CAMERA_PERSPECTIVE;
    world->camera.target.x   = 0;
    world->camera.target.y   = 0;
    world->camera.target.z   = 0;

    physics_s* physics = ecs.components.add<physics_s>(entity);
    physics->engine    = PhysicsEngine();

    return entity;
}

entity_id create_ball(ecs_s& ecs, float cx, float cy, float cz, float r) {
    entity_id entity  = ecs.entities.create();
    auto [_, physics] = ecs.components.first<physics_s>();
    auto& interface   = physics->engine.Interface();

    body_s* body                   = ecs.components.add<body_s>(entity);
    body->mesh                     = R3D_GenMeshSphere(r, 32, 32, true);
    body->material                 = R3D_GetDefaultMaterial();
    body->material.emission.color  = RED;
    body->material.emission.energy = 1.0;

    JPH::SphereShape*         sphere_shape = new JPH::SphereShape(r);
    JPH::BodyCreationSettings sphere_settings(sphere_shape, JPH::RVec3 {cx, cy, cz}, JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
    sphere_settings.mRestitution = 0.5f; // A bit of bounce
    body->body_id                = interface.CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
    return entity;
}

entity_id create_maze(ecs_s& ecs, float cx, float cy, float cz, float w, float h, float l) {
    entity_id entity  = ecs.entities.create();
    auto [_, physics] = ecs.components.first<physics_s>();
    auto& interface   = physics->engine.Interface();

    body_s* body                   = ecs.components.add<body_s>(entity);
    body->mesh                     = R3D_GenMeshCube(w, h, l, true);
    body->material                 = R3D_GetDefaultMaterial();
    body->material.emission.color  = BLUE;
    body->material.emission.energy = 1.0;

    JPH::BoxShape*            box_shape = new JPH::BoxShape(JPH::Vec3(w / 2, h / 2, l / 2));
    JPH::BodyCreationSettings box_settings(box_shape, JPH::RVec3 {cx, cy, cz}, JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::MOVING);
    body->body_id = interface.CreateAndAddBody(box_settings, JPH::EActivation::Activate);

    return entity;
}

entity_id create_light(ecs_s& ecs, float x, float y, float z) {
    entity_id entity = ecs.entities.create();

    light_s* light = ecs.components.add<light_s>(entity);
    // TODO The light needs to be manually destroyed using R3D_DestroyLight
    light->light = R3D_CreateLight(R3D_LIGHT_OMNI);
    R3D_SetLightActive(light->light, true);
    R3D_SetLightPosition(light->light, {x, y, z});

    return entity;
}