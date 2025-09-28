#pragma once

#include "components.hpp"

void draw_system(body_s* body);
void move_system(ecs_s& ecs);
void player_input_system(body_s* body);