#pragma once
#include "ecs/System.h"

class UI : public System
{
public:
    
    void update(World& world, float deltaTime) override;
};
