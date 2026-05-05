#pragma once
#include <unordered_set>
#include "ecs/Types.h"
struct EnemyComponent
{
    float speed;
    float Damage;
    std::unordered_set<Entity> visitedTargets;
};
