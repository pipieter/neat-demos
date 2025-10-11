#pragma once

#include <raylib.h>

#include "components.hpp"

class engine_s {
   private:
    ecs_s           ecs;
    RenderTexture2D render;

   public:
    engine_s();
    ~engine_s();

    void run();
};