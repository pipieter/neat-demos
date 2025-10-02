#pragma once

#include "components.hpp"

void draw_system(ecs_s& ecs);
void physics_system(world_s* world);
void player_input_system(body_s* body, player_s*);
void shoot_system(ecs_s& ecs, body_s* body, player_s* player);
void draw_aim_system(player_s* player);
void bullet_enemy_collision_system(ecs_s& ecs, world_s* world);
void debug_system(world_s* world);
void debug_draw_system(ecs_s& ecs, world_s* world);