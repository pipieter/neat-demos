#include "entities.hpp"

#include <stdexcept>

#include "assets.hpp"

float world_ambient[4] = {0.4f, 0.4f, 0.4f, 1.0f};

entity_id create_world(ecs_s& ecs) {
    entity_id entity = ecs.entities.create();

    world_s* world = ecs.components.add<world_s>(entity);
    world->ecs     = &ecs;

    // Create shader
    world->shader                              = LoadShaderFromMemory(vert_shader, frag_shader);
    world->shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(world->shader, "viewPos");
    int ambient_loc                            = GetShaderLocation(world->shader, "ambient");
    SetShaderValue(world->shader, ambient_loc, world_ambient, SHADER_UNIFORM_VEC4);

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

    return entity;
}

entity_id create_ball(ecs_s& ecs, float cx, float cy, float cz, float r) {
    entity_id entity = ecs.entities.create();

    body_s* body   = ecs.components.add<body_s>(entity);
    body->type     = body_type::sphere;
    body->sphere.x = cx;
    body->sphere.y = cy;
    body->sphere.z = cz;
    body->sphere.r = r;

    return entity;
}

entity_id create_maze(ecs_s& ecs, float cx, float cy, float cz, float w, float h, float l) {
    entity_id entity = ecs.entities.create();

    body_s* body = ecs.components.add<body_s>(entity);
    body->type   = body_type::box;
    body->box.x  = cx - w / 2;
    body->box.y  = cy - h / 2;
    body->box.z  = cz - l / 2;
    body->box.w  = w;
    body->box.h  = h;
    body->box.l  = l;

    return entity;
}

const int max_lights_count = 4;
int       lights_count     = 0;

entity_id create_light(ecs_s& ecs, const Shader& shader, float x, float y, float z) {
    if (lights_count >= max_lights_count) {
        throw std::logic_error("Too many lights have been added.");
    }

    entity_id entity = ecs.entities.create();

    light_s* light = ecs.components.add<light_s>(entity);

    light->enabled  = true;
    light->type     = light_type::point;
    light->position = {x, y, z};
    light->target   = {0, 0, 0};
    light->color    = WHITE;

    // NOTE: Lighting shader naming must be the provided ones
    light->enabled_loc  = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lights_count));
    light->type_loc     = GetShaderLocation(shader, TextFormat("lights[%i].type", lights_count));
    light->position_loc = GetShaderLocation(shader, TextFormat("lights[%i].position", lights_count));
    light->target_loc   = GetShaderLocation(shader, TextFormat("lights[%i].target", lights_count));
    light->color_loc    = GetShaderLocation(shader, TextFormat("lights[%i].color", lights_count));

    lights_count++;

    return entity;
}