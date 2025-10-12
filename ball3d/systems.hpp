#pragma once

#include "components.hpp"

void physics_system(physics_s* physics);
void player_input_system(world_s* world);
void draw_system(ecs_s& ecs);

// Debug tools for testing
void debug_ball_respawn(body_s* body, ball_s* ball);
void debug_wall_rotation_system(body_s* body, wall_s* wall);  