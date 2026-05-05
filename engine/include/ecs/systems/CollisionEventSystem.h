#pragma once
#include "ecs/Types.h"
#include "ecs/events/CollisionDirection.h"

struct CollisionEvent
{
    Entity a;
    Entity b;
    CollisionDirection direction = CollisionDirection::None;
};
