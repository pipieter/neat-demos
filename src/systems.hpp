#pragma once

#include "components.hpp"

void draw_system(body_s* body);
void move_system(body_s* body, velocity_s* velocity);
void player_input_system(ecs_s& ecs, entity_id entity, player_s*);