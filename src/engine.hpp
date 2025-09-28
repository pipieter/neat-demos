#pragma once

#include "components.hpp"

class engine_s {
   private:
    ecs_s ecs;

   public:
    engine_s();
    ~engine_s();

    void run();
};