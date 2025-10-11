#include "common.hpp"

#include <iostream>

Rectangle get_screen_bounds() {
    float screen_x            = 0;
    float screen_y            = 0;
    float screen_width        = (float)GetScreenWidth();
    float screen_height       = (float)GetScreenHeight();
    float screen_aspect_ratio = screen_width / screen_height;

    if (screen_aspect_ratio > render_aspect_ratio) {
        // Aspect ratio is wider, limit the screen_width and adjust screen_x
        float new_screen_width = render_aspect_ratio * screen_height;
        screen_x               = (screen_width - new_screen_width) / 2;
        screen_width           = new_screen_width;
    } else if (screen_aspect_ratio < render_aspect_ratio) {
        // Aspect ratio is taller limit the screen_height and adjust screen_y
        float new_screen_height = screen_width / render_aspect_ratio;
        screen_y                = (screen_height - new_screen_height) / 2;
        screen_height           = new_screen_height;
    }

    return Rectangle {screen_x, screen_y, screen_width, screen_height};
}

b2Vec2 screen_to_world_space(b2Vec2 screen_position) {
    // Note: because the screen is layered through the render texture, we also
    // need to take the offsets and bounds into account.
    auto bounds = get_screen_bounds();

    // Screen to render
    float render_x = (screen_position.x - bounds.x) / bounds.width;
    float render_y = (screen_position.y - bounds.y) / bounds.height;

    // Render to world
    float world_x = render_x * world_width;
    float world_y = render_y * world_height;

    return b2Vec2 {world_x, world_y};
}

b2Vec2 world_to_screen_space(b2Vec2 world_position) {
    float screen_x = world_position.x * render_width / world_width;
    float screen_y = world_position.y * render_height / world_height;

    return b2Vec2 {screen_x, screen_y};
}