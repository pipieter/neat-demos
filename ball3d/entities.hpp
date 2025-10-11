#pragma once

#include "components.hpp"

entity_id create_world(ecs_s& ecs);
entity_id create_ball(ecs_s& ecs, float cx, float cy, float cz, float r);
entity_id create_maze(ecs_s& ecs, float cx, float cy, float cz, float w, float h, float l);
entity_id create_light(ecs_s& ecs, const Shader& shader, float x, float y, float z);