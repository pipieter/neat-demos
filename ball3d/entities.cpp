#include "entities.hpp"

#include <stdexcept>

#include "maze.hpp"

float world_ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};

entity_id create_world(ecs_s& ecs) {
    entity_id entity = ecs.entities.create();

    world_s* world  = ecs.components.add<world_s>(entity);
    world->ecs      = &ecs;
    world->position = 0.1f;

    // Create camera
    world->camera.up.x       = 0;
    world->camera.up.y       = 1;
    world->camera.up.z       = 0;
    world->camera.fovy       = 45.0f;
    world->camera.position.x = -0.01f;
    world->camera.position.y = +12.f;
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
    mesh->material.emission.color  = {255, 0, 0, 255};
    mesh->material.emission.energy = 0.75;

    body_s*                   body         = ecs.components.add<body_s>(entity);
    JPH::SphereShape*         sphere_shape = new JPH::SphereShape(r);
    JPH::BodyCreationSettings sphere_settings(sphere_shape, JPH::RVec3 {cx, cy, cz}, JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
    sphere_settings.mRestitution = 0.0f;  // No bounce

    body->interface = interface;
    body->id        = interface->CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);

    (void)ecs.components.add<ball_s>(entity);

    return entity;
}

entity_id create_maze(ecs_s& ecs, unsigned short w, unsigned short h) {
    const float thickness = 0.2;

    auto [_, physics] = ecs.components.first<physics_s>();
    auto interface    = physics->engine.Interface();

    auto    entity = ecs.entities.create();
    body_s* body   = ecs.components.add<body_s>(entity);
    mesh_s* mesh   = ecs.components.add<mesh_s>(entity);
    maze_s  maze   = maze_s(w, h, 999);

    JPH::MutableCompoundShapeSettings compound_settings;

    // Add floor
    JPH::BoxShapeSettings* floor_shape = new JPH::BoxShapeSettings(JPH::RVec3 {(float)w / 2, thickness / 2, (float)h / 2});
    compound_settings.AddShape(JPH::RVec3 {0.0f, 0.0f, 0.0f}, JPH::Quat::sIdentity(), floor_shape);

    // outer walls
    float wall_h = 0.4;
    float wall_y = wall_h / 2 + thickness / 2;

    JPH::BoxShapeSettings* lr_wall_shape  = new JPH::BoxShapeSettings(JPH::RVec3 {thickness / 2, wall_h / 2, (float)h / 2 + thickness / 2});  // Left/right wall
    JPH::BoxShapeSettings* tb_bwall_shape = new JPH::BoxShapeSettings(JPH::RVec3 {(float)w / 2 + thickness / 2, wall_h / 2, thickness / 2});  // Top/bottom wall
    compound_settings.AddShape(JPH::RVec3 {(float)-w / 2, wall_y, 0.0f}, JPH::Quat::sIdentity(), lr_wall_shape);
    compound_settings.AddShape(JPH::RVec3 {(float)+w / 2, wall_y, 0.0f}, JPH::Quat::sIdentity(), lr_wall_shape);
    compound_settings.AddShape(JPH::RVec3 {0.0f, wall_y, (float)-h / 2}, JPH::Quat::sIdentity(), tb_bwall_shape);
    compound_settings.AddShape(JPH::RVec3 {0.0f, wall_y, (float)+h / 2}, JPH::Quat::sIdentity(), tb_bwall_shape);

    // Add maze walls
    for (const auto& edge : maze.edges) {
        auto a = std::get<0>(edge);
        auto b = std::get<1>(edge);

        float x_a = (float)std::get<0>(a);
        float y_a = (float)std::get<1>(a);
        float x_b = (float)std::get<0>(b);
        float y_b = (float)std::get<1>(b);

        float wall_w = std::max(std::abs(y_a - y_b), 0.0f) + thickness;
        float wall_l = std::max(std::abs(x_a - x_b), 0.0f) + thickness;

        float wall_x = (x_a + x_b - (float)w + 1.0) / 2.0f;
        float wall_z = (y_a + y_b - (float)h + 1.0) / 2.0f;

        JPH::BoxShapeSettings* wall_shape = new JPH::BoxShapeSettings(JPH::RVec3 {wall_w / 2, wall_h / 2, wall_l / 2});
        compound_settings.AddShape(JPH::RVec3 {wall_x, wall_y, wall_z}, JPH::Quat::sIdentity(), wall_shape);
    }

    JPH::Shape*               shape = compound_settings.Create().Get();
    JPH::BodyCreationSettings body_creation(shape, JPH::RVec3 {0, 0, 0}, JPH::Quat::sIdentity(), JPH::EMotionType::Kinematic, Layers::NON_MOVING);

    body->interface = interface;
    body->id        = interface->CreateAndAddBody(body_creation, JPH::EActivation::Activate);

    mesh->mesh                     = GenMeshFromShape(shape, true);
    mesh->material                 = R3D_GetDefaultMaterial();
    mesh->material.emission.color  = DARKBROWN;
    mesh->material.emission.energy = 0.75;

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
    R3D_EnableShadow(light->light, 1024);
    R3D_SetLightEnergy(light->light, 1.0);
    R3D_SetLightColor(light->light, LIGHTGRAY);

    return entity;
}