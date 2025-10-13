#pragma once

#include "components.hpp"

void remove_entity(ecs_s& ecs, entity_id entity);

entity_id create_world(ecs_s& ecs);
entity_id create_ball(ecs_s& ecs, float cx, float cy, float cz, float r);
entity_id create_maze(ecs_s& ecs, unsigned short w, unsigned short h, size_t seed);
entity_id create_light(ecs_s& ecs, float x, float y, float z);
