#pragma once
#include "ecs/System.h"

class VFXSystem : public System
{
public:
    void update(World& world, float deltaTime) override;
};
