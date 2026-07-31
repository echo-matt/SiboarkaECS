#pragma once
#include "components/WaveStateComponent.h"
#include "ecs/System.h"

class WaveSpawnerSystem : System
{
public:
    WaveSpawnerSystem(float screenWidth, float screenHeight);
    void update(World& world, float deltaTime) override;
    bool bFinishedSpawning = false;
private:
    float _spawnTimer = 1.f;
    float _screenW, _screenH;
};
