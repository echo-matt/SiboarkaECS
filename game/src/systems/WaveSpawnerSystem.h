#pragma once
#include "ecs/System.h"

class WaveSpawnerSystem : System
{
public:
    WaveSpawnerSystem(float screenWidth, float screenHeight);
    void update(World& world, float deltaTime) override;
private:
    float _spawnTimer = 1.f;
    float enemyNumber;
    float _screenW, _screenH;
};
