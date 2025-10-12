#pragma once

#include "components.hpp"

entity_id create_world(ecs_s& ecs);
entity_id create_ball(ecs_s& ecs, float cx, float cy, float cz, float r);
entity_id create_wall(ecs_s& ecs, float cx, float cy, float cz, float w, float h, float l);
entity_id create_box(ecs_s& ecs, float cx, float cy, float cz, float w, float h, float l, float thickness);
entity_id create_light(ecs_s& ecs, float x, float y, float z);