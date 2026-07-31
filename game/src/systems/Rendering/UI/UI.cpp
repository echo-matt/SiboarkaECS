#include "UI.h"

#include <format>
#include <string>

#include "components/BaseComponent.h"
#include "components/HealthComponent.h"
#include "ecs/Types.h"
#include "ecs/World.h"
#include "raylib.h"
#include "components/EnemyComponent.h"
#include "components/GameStateComponent.h"
#include "ecs/components/TransformComponent.h"
#include "systems/WaveSpawner/WaveSpawnerSystem.h"

struct TransformComponent;

void UI::update(World& world, float deltaTime)
{
    
    for (Entity e : world.getEntitiesWith<BaseComponent>())
    {
        HealthComponent baseHealth =  world.getComponent<HealthComponent>(e);
            
        DrawText(std::format("HP: {}", baseHealth.currentHP).c_str(), 0, 880, 20, RED);
    }
    
    for (Entity e : world.getEntitiesWith<WaveStateComponent>())
    {
        WaveStateComponent WS = world.getComponent<WaveStateComponent>(e);
        DrawText(std::format("Wave: {}", WS.waveNumber+1).c_str(), 100, 880, 20, RED);
        DrawText(std::format("Enemy number: {}", WS.enemyNumber).c_str(), 200, 880, 20, RED);
    }
    
    
}
