#pragma once

#include <raylib.h>

struct Sprite2D {
    Texture2D texture;
    int       x;
    int       y;
    int       width;
    int       height;

    Rectangle bounds() const;
};

namespace assets {

void load();
void unload();

extern Texture2D level01_image;
extern Texture2D sprites_image;
extern Sprite2D  player_sprite;
extern Sprite2D  enemy_sprite;
extern Sprite2D  bullet_sprite;

};  // namespace assets