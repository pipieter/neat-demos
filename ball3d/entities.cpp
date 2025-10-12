#include "entities.hpp"

#include <stdexcept>

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
    world->camera.position.x = -10.0f;
    world->camera.position.y = 10.0f;
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
    auto interface    = physics->engine.Interface();

    mesh_s* mesh                   = ecs.components.add<mesh_s>(entity);
    mesh->mesh                     = R3D_GenMeshSphere(r, 32, 32, true);
    mesh->material                 = R3D_GetDefaultMaterial();
    mesh->material.emission.color  = RED;
    mesh->material.emission.energy = 1.0;

    body_s*                   body         = ecs.components.add<body_s>(entity);
    JPH::SphereShape*         sphere_shape = new JPH::SphereShape(r);
    JPH::BodyCreationSettings sphere_settings(sphere_shape, JPH::RVec3 {cx, cy, cz}, JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
    sphere_settings.mRestitution = 0.1f;  // No bounce

    body->interface = interface;
    body->id        = interface->CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);

    (void)ecs.components.add<ball_s>(entity);

    return entity;
}

entity_id create_wall(ecs_s& ecs, float cx, float cy, float cz, float w, float h, float l) {
    entity_id entity  = ecs.entities.create();
    auto [_, physics] = ecs.components.first<physics_s>();
    auto interface    = physics->engine.Interface();

    mesh_s* mesh                   = ecs.components.add<mesh_s>(entity);
    mesh->mesh                     = R3D_GenMeshCube(w, h, l, true);
    mesh->material                 = R3D_GetDefaultMaterial();
    mesh->material.emission.color  = BLUE;
    mesh->material.emission.energy = 1.0;

    body_s*                   body      = ecs.components.add<body_s>(entity);
    JPH::BoxShape*            box_shape = new JPH::BoxShape(JPH::Vec3(w / 2, h / 2, l / 2));
    JPH::BodyCreationSettings box_settings(box_shape, JPH::RVec3 {cx, cy, cz}, JPH::Quat::sIdentity(), JPH::EMotionType::Kinematic, Layers::NON_MOVING);
    // Prevents the wall from rotating when attached to a moving platform. This is somewhat hacky, but
    // should be fine for the purposes of this demo.
    box_settings.mInertiaMultiplier = 1e10;

    body->interface = interface;
    body->id        = interface->CreateAndAddBody(box_settings, JPH::EActivation::Activate);

    return entity;
}

void constrain_body(JPH::PhysicsSystem* physics_system, const JPH::BodyLockInterface* bli, const JPH::BodyID& body1_id, const JPH::BodyID& body2_id) {
    JPH::FixedConstraintSettings settings;
    settings.mAutoDetectPoint = true;  // attach at current relative pose
    settings.mSpace           = JPH::EConstraintSpace::WorldSpace;
    settings.mEnabled         = true;

    JPH::Body* body1 = bli->TryGetBody(body1_id);
    JPH::Body* body2 = bli->TryGetBody(body2_id);
    body2->SetMotionType(JPH::EMotionType::Dynamic);
    JPH::TwoBodyConstraint* constraint = settings.Create(*body1, *body2);
    constraint->SetEnabled(true);

    physics_system->AddConstraint(constraint);
}

entity_id create_box(ecs_s& ecs, float cx, float cy, float cz, float w, float h, float l, float thickness) {
    auto [_, physics]   = ecs.components.first<physics_s>();
    auto lock_interface = physics->engine.LockInterface();
    auto physics_system = physics->engine.PhysicsSystem();

    entity_id floor = create_wall(ecs, cx, cy - h / 2 - thickness / 2, cz, w + 2 * thickness, thickness, l + 2 * thickness);
    entity_id wall1 = create_wall(ecs, cx, cy, cz + l / 2 + thickness / 2, w + 2 * thickness, h + 2 * thickness, thickness);  // Top wall
    entity_id wall2 = create_wall(ecs, cx, cy, cz - l / 2 - thickness / 2, w + 2 * thickness, h + 2 * thickness, thickness);  // Bottom wall
    entity_id wall3 = create_wall(ecs, cx - w / 2 - thickness / 2, cy, cz, thickness, h + 2 * thickness, l + 2 * thickness);  // Left wall
    entity_id wall4 = create_wall(ecs, cx + w / 2 + thickness / 2, cy, cz, thickness, h + 2 * thickness, l + 2 * thickness);  // Right wall

    body_s* floor_body = ecs.components.get<body_s>(floor);
    body_s* wall1_body = ecs.components.get<body_s>(wall1);
    body_s* wall2_body = ecs.components.get<body_s>(wall2);
    body_s* wall3_body = ecs.components.get<body_s>(wall3);
    body_s* wall4_body = ecs.components.get<body_s>(wall4);

    constrain_body(physics_system.get(), lock_interface, floor_body->id, wall1_body->id);
    constrain_body(physics_system.get(), lock_interface, floor_body->id, wall2_body->id);
    constrain_body(physics_system.get(), lock_interface, floor_body->id, wall3_body->id);
    constrain_body(physics_system.get(), lock_interface, floor_body->id, wall4_body->id);

    (void)ecs.components.add<rotation_s>(floor);

    return floor;
}

entity_id create_light(ecs_s& ecs, float x, float y, float z) {
    entity_id entity = ecs.entities.create();

    light_s* light = ecs.components.add<light_s>(entity);
    // TODO The light needs to be manually destroyed using R3D_DestroyLight
    light->light = R3D_CreateLight(R3D_LIGHT_OMNI);
    R3D_SetLightActive(light->light, true);
    R3D_SetLightPosition(light->light, {x, y, z});
    R3D_EnableShadow(light->light, 4096);

    return entity;
}