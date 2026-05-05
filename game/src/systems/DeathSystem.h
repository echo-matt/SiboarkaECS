#pragma once
#include "ecs/System.h"

class DeathSystem : public System
{
public:
    void update(World& world, float deltaTime) override;
};
