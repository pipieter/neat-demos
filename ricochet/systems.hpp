#pragma once

#include "components.hpp"

void draw_system(ecs_s& ecs);
void physics_system(world_s* world);
void player_input_system(body_s* body, player_s*);
void player_aim_flip_system(player_s* player, sprite_s* sprite);
void shoot_system(ecs_s& ecs, body_s* body, player_s* player);
void draw_aim_system(player_s* player);
void bullet_enemy_collision_system(ecs_s& ecs, world_s* world);
void bullet_wall_collision_system(ecs_s& ecs, world_s* world);
void bullet_destroy_system(ecs_s& ecs);
void debug_system(world_s* world);
void debug_draw_system(ecs_s& ecs, world_s* world);

void deletion_system(ecs_s& ecs);