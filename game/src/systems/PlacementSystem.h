#pragma once
#include "ecs/System.h"
#include "ecs/Types.h"

class PlacementSystem : public System
{
public:
    PlacementSystem(float screenW, float screenH, Entity (&grid)[30][30]);
    void update(World& world, float deltaTime) override;
    
private:
    Entity (&_grid)[30][30];
    float _screenW, _screenH;
};
