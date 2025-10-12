#pragma once

#include "components.hpp"

void physics_system(physics_s* physics);
void draw_system(ecs_s& ecs);
void ball_reset_system(body_s* body, ball_s* ball);
void platform_rotation_system(body_s* body, rotation_s* rotation);
void log_enable_system(ecs_s& ecs);