#include "components.hpp"

void remove_entity(ecs_s* ecs, entity_id entity);

entity_id create_world(ecs_s* ecs);
entity_id create_player(ecs_s* ecs, float x, float y);
entity_id create_wall(ecs_s* ecs, float x, float y, float w, float h);
entity_id create_bullet(ecs_s* ecs, float x, float y, float dx, float dy);
entity_id create_enemy(ecs_s* ecs, float x, float y);