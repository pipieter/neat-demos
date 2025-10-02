
#include "assets.hpp"
#include "engine.hpp"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "Ricochet");
    SetTargetFPS(60);

    assets::load();

    engine_s engine;
    engine.run();

    assets::unload();

    CloseWindow();

    return 0;
}