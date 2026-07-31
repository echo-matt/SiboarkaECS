#pragma once
#include "ecs/System.h"

class WaveSpawnerSystem : System
{
public:
    WaveSpawnerSystem(float screenWidth, float screenHeight);
    void update(World& world, float deltaTime) override;
    bool bFinishedSpawning = false;
private:
    float _spawnTimer = 1.f;
    int enemyNumber = 10;
    int totalWavesToSpawn = 1;
    float _screenW, _screenH;

};
