#pragma once

#include <box2d/box2d.h>

const float unit = 1.0f;

const float world_width  = 32.0f;
const float world_height = 18.0f;

const float render_width        = 1920.0f;
const float render_height       = 1080.0f;
const float render_aspect_ratio = render_width / render_height;

const float render_scale = render_width / world_width;

b2Vec2 screen_to_world_space(b2Vec2 vec);
b2Vec2 world_to_screen_space(b2Vec2 vec);