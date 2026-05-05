#pragma once
#include "ecs/System.h"

class PlacementSystem : public System
{
public:
    PlacementSystem(float screenW, float screenH, bool (&grid)[30][30]);
    void update(World& world, float deltaTime) override;
    
private:
    bool (&_grid)[30][30];
    float _screenW, _screenH;
};
