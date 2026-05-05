#pragma once
#include "ecs/System.h"

class ShootingSystem : public System
{
public:
    void update(World& world, float deltaTime);
};