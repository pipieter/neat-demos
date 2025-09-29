#include "components.hpp"

b2Vec2 body_s::center() const {
    return b2Body_GetPosition(b2_id);
}