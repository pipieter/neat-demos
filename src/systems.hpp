#pragma once

#include "components.hpp"

void draw_system(transform_s* transform);
void move_system(ecs_st& ecs);
void player_input_system(ecs_st& ecs, entity_id entity, player_s* player);