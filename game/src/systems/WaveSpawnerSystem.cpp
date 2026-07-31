#include "WaveSpawnerSystem.h"

#include <format>

#include "components/EnemyComponent.h"
#include "components/GameStateComponent.h"
#include "components/HealthComponent.h"
#include "components/RenderComponent.h"
#include "ecs/Logger.h"
#include "ecs/World.h"
#include "ecs/components/ColliderComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "events/FinishedSpawningEvent.h"

WaveSpawnerSystem::WaveSpawnerSystem(float screenWidth, float screenHeight)
{
    _screenW = screenWidth;
    _screenH = screenHeight;
}

void WaveSpawnerSystem::update(World& world, float deltaTime)
{
    if (!bFinishedSpawning)
    {
        _spawnTimer -= deltaTime;
        float randX = std::rand() % (int)(_screenW + 1);
        float randY = std::rand() % (int)(_screenH + 1);

        if (_spawnTimer <= 0)
        {
            Entity enemy = world.createEntity();
            world.addComponent(enemy, TransformComponent{randX, randY, 50.f, 50.f});
            world.addComponent(enemy, EnemyComponent{70.f, 25.f});
            world.addComponent(enemy, HealthComponent{100, 100});
            world.addComponent(enemy, RenderComponent{15, 15, RED});
            world.addComponent(enemy, ColliderComponent{15, 15, false});
            world.addComponent(enemy, TagComponent{"Enemy"});

            SIBOLOG_DEBUG(std::format("Enemy spawned at x: {}, y: {}", world.getComponent<TransformComponent>(enemy).x,world.getComponent<TransformComponent>(enemy).y));

            _spawnTimer = 1.f;
            enemyNumber--;
            SIBOLOG_DEBUG(std::format("Enemy number: {}", enemyNumber));
        }

        if (enemyNumber == 0)
        {
            totalWavesToSpawn--;
            enemyNumber = 10;
            SIBOLOG_DEBUG(std::format("Waves to spawn: ", totalWavesToSpawn));
            if (totalWavesToSpawn == 0)
            {
                SIBOLOG_DEBUG(std::format("All waves spawned, finished spawning"));
                bFinishedSpawning = true;
                world.events.emit(FinishedSpawningEvent{});
                
                for (Entity& e : world.getEntitiesWith<GameStateComponent>())
                {
                    world.getComponent<GameStateComponent>(e).allWavesSpawned = true;
                }
            }
        }
    }
}
