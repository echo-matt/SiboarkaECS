#pragma once
#include <unordered_set>
#include "ecs/Types.h"
struct EnemyComponent
{
    float speed;
    float Damage;
    float attackCooldown = 0.f;
    float attackRate    = 1.f;
    std::unordered_set<Entity> visitedTargets;
};
