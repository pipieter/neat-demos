#include "components.hpp"

void create_world(ecs_s* ecs);
void create_player(ecs_s* ecs, float x, float y);
void create_wall(ecs_s* ecs, float x, float y, float w, float h);
void create_bullet(ecs_s* ecs, float x, float y, float dx, float dy);