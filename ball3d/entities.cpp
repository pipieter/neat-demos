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
    world->camera.position.x = -5.0f;
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

    body->interface = interface;
    body->id        = interface->CreateAndAddBody(box_settings, JPH::EActivation::Activate);

    return entity;
}

entity_id create_box(ecs_s& ecs, float cx, float cy, float cz, float w, float h, float l, float thickness) {
    auto [_, physics] = ecs.components.first<physics_s>();
    auto interface    = physics->engine.Interface();

    auto    entity = ecs.entities.create();
    body_s* body   = ecs.components.add<body_s>(entity);
    mesh_s* mesh   = ecs.components.add<mesh_s>(entity);

    JPH::BoxShapeSettings* floor_shape = new JPH::BoxShapeSettings(JPH::RVec3 {w / 2 + thickness, thickness / 2, l / 2 + thickness});
    JPH::BoxShapeSettings* twall_shape = new JPH::BoxShapeSettings(JPH::RVec3 {w / 2 + thickness, h / 2 + thickness, thickness / 2});  // Top wall
    JPH::BoxShapeSettings* bwall_shape = new JPH::BoxShapeSettings(JPH::RVec3 {w / 2 + thickness, h / 2 + thickness, thickness / 2});  // Bottom wall
    JPH::BoxShapeSettings* lwall_shape = new JPH::BoxShapeSettings(JPH::RVec3 {thickness / 2, h / 2 + thickness, l / 2 + thickness});  // Left wall
    JPH::BoxShapeSettings* rwall_shape = new JPH::BoxShapeSettings(JPH::RVec3 {thickness / 2, h / 2 + thickness, l / 2 + thickness});  // Right wall

    JPH::MutableCompoundShapeSettings compound_settings;
    compound_settings.AddShape(JPH::RVec3 {0, -h / 2 - thickness / 2, 0}, JPH::Quat::sIdentity(), floor_shape);
    compound_settings.AddShape(JPH::RVec3 {0, 0, +l / 2 + thickness / 2}, JPH::Quat::sIdentity(), twall_shape);
    compound_settings.AddShape(JPH::RVec3 {0, 0, -l / 2 - thickness / 2}, JPH::Quat::sIdentity(), bwall_shape);
    compound_settings.AddShape(JPH::RVec3 {-w / 2 - thickness / 2, 0, 0}, JPH::Quat::sIdentity(), lwall_shape);
    compound_settings.AddShape(JPH::RVec3 {+w / 2 + thickness / 2, 0, 0}, JPH::Quat::sIdentity(), rwall_shape);

    JPH::Shape*               shape = compound_settings.Create().Get();
    JPH::BodyCreationSettings body_creation(shape, JPH::RVec3 {cx, cy, cz}, JPH::Quat::sIdentity(), JPH::EMotionType::Kinematic, Layers::NON_MOVING);

    body->interface = interface;
    body->id        = interface->CreateAndAddBody(body_creation, JPH::EActivation::Activate);

    mesh->mesh                     = GenMeshFromShape(shape, true);
    mesh->material                 = R3D_GetDefaultMaterial();
    mesh->material.emission.color  = BLUE;
    mesh->material.emission.energy = 1.0;

    (void)ecs.components.add<rotation_s>(entity);

    return entity;
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