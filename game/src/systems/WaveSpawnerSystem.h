#pragma once
#include "ecs/System.h"

class WaveSpawnerSystem : System
{
public:
    void update(World& world, float deltaTime) override;
private:
    float _spawnTimer = 2.f;
    float enemyNumber;
};
