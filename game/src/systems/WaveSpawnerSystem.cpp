#include "WaveSpawnerSystem.h"

#include <format>

#include "components/EnemyComponent.h"
#include "components/HealthComponent.h"
#include "components/RenderComponent.h"
#include "ecs/Logger.h"
#include "ecs/World.h"
#include "ecs/components/ColliderComponent.h"
#include "ecs/components/TransformComponent.h"

WaveSpawnerSystem::WaveSpawnerSystem(float screenWidth, float screenHeight)
{
    _screenW = screenWidth;
    _screenH = screenHeight;
}

void WaveSpawnerSystem::update(World& world, float deltaTime)
{
    _spawnTimer -= deltaTime;
    float randX = std::rand() % (int)(_screenW+1);
    float randY = std::rand() % (int)(_screenH+1);
    
    if (_spawnTimer <= 0)
    {
        
        Entity enemy = world.createEntity();
        world.addComponent(enemy, TransformComponent{randX, randY, 50.f, 50.f});
        world.addComponent(enemy, EnemyComponent{70.f, 10.f});
        world.addComponent(enemy, HealthComponent{100, 100});
        world.addComponent(enemy, RenderComponent{15, 15, RED});
        world.addComponent(enemy, ColliderComponent{15, 15, false});
        
        SIBOLOG_DEBUG(std::format("Enemy spawned at x: {}, y: {}", world.getComponent<TransformComponent>(enemy).x,world.getComponent<TransformComponent>(enemy).y));
        
        _spawnTimer = 1.f;
    }
}
