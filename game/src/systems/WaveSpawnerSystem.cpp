#include "WaveSpawnerSystem.h"

#include "components/EnemyComponent.h"
#include "components/HealthComponent.h"
#include "components/RenderComponent.h"
#include "ecs/World.h"
#include "ecs/components/ColliderComponent.h"
#include "ecs/components/TransformComponent.h"

void WaveSpawnerSystem::update(World& world, float deltaTime)
{
    _spawnTimer -= deltaTime;

    if (_spawnTimer <= 0)
    {
        
        Entity enemy = world.createEntity();
        world.addComponent(enemy, TransformComponent{100.f, 100.f, 50.f, 50.f});
        world.addComponent(enemy, EnemyComponent{1.f, 200.f});
        world.addComponent(enemy, HealthComponent{100, 100});
        world.addComponent(enemy, RenderComponent{15, 15, RED});
        world.addComponent(enemy, ColliderComponent{100, 100, false});
        
        _spawnTimer = 2.f;
    }
}
