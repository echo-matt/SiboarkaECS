#include "GameStateSystem.h"

#include <format>
#include <components/BaseComponent.h>

#include "components/EnemyComponent.h"
#include "components/GameStateComponent.h"
#include "components/HealthComponent.h"
#include "ecs/Logger.h"
#include "ecs/Types.h"
#include "ecs/World.h"
#include "events/FinishedSpawningEvent.h"
#include "events/GameOverEvent.h"

void GameStateSystem::update(World& world, float deltaTime)
{
    for (Entity& e : world.getEntitiesWith<BaseComponent, HealthComponent>())
    {
        HealthComponent& healthComp = world.getComponent<HealthComponent>(e);
        if (healthComp.currentHP <= 0)
        {
            for (Entity& entity : world.getEntitiesWith<GameStateComponent>())
            {
                bGameOver = true;
                world.getComponent<GameStateComponent>(entity).gameOver = true;
                world.events.emit(gameOverEvent{});
            }
        }
    }
    
    for (Entity& entity : world.getEntitiesWith<GameStateComponent>())
    {
        if (world.getComponent<GameStateComponent>(entity).allWavesSpawned && world.getEntitiesWith<EnemyComponent>().empty())
        {
            bGameOver = true;
            world.getComponent<GameStateComponent>(entity).gameWon = true;
            world.events.emit(gameOverEvent{});
        }
    }
}

