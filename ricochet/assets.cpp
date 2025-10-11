#include "assets.hpp"

Rectangle Sprite2D::bounds() const {
    return {(float)x, (float)y, (float)width, (float)height};
}

Texture2D assets::level01_image;
Texture2D assets::sprites_image;
Sprite2D  assets::player_sprite;
Sprite2D  assets::enemy_sprite;
Sprite2D  assets::bullet_sprite;

void assets::load() {
    level01_image = LoadTexture("assets/images/level01.png");
    sprites_image = LoadTexture("assets/images/sprites.png");

    player_sprite = {sprites_image, 0, 0, 8, 8};
    enemy_sprite  = {sprites_image, 0, 8, 8, 8};
    bullet_sprite = {sprites_image, 3, 19, 2, 2};
}

void assets::unload() {
    UnloadTexture(level01_image);
    UnloadTexture(sprites_image);
}