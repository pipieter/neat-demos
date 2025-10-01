#pragma once

#include "components.hpp"

void draw_system(body_s* body);
void move_system(world_s* world);
void player_input_system(body_s* body, player_s*);
void shoot_system(ecs_s& ecs, body_s* body, player_s* player);
void draw_aim_system(player_s* player);