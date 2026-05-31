#include "DamageSystem.h"

#include <format>

#include "components/BaseComponent.h"
#include "components/BulletComponent.h"
#include "components/DeadComponent.h"
#include "components/EnemyComponent.h"
#include "components/HealthComponent.h"
#include "components/TowerComponent.h"
#include "ecs/Logger.h"
#include "ecs/World.h"
#include "ecs/systems/CollisionEventSystem.h"
#include "events/ShotFiredEvent.h"

void DamageSystem::update(World& world, float deltaTime)
{
    // Tick every enemy's attack cooldown down each frame so they can attack again.
    for (Entity e : world.getEntitiesWith<EnemyComponent>())
    {
        auto& enemy = world.getComponent<EnemyComponent>(e);
        if (enemy.attackCooldown > 0.f)
            enemy.attackCooldown -= deltaTime;
    }
    auto& collisionEvents = world.events.getEvents<CollisionEvent>();
    
    //Hitscan
    for (const auto& shotFiredEvent : world.events.getEvents<ShotFiredEvent>())
    {
        if (world.isAlive(shotFiredEvent.targetEntity))
        {
            world.getComponent<HealthComponent>(shotFiredEvent.targetEntity).currentHP -= world.getComponent<TowerComponent>(shotFiredEvent.sourceEntity).damage;
        }
    }
    
     for (const auto& collisionEvent : collisionEvents)
    {
        if (world.hasComponent<EnemyComponent>(collisionEvent.a))
        {
            auto& enemy = world.getComponent<EnemyComponent>(collisionEvent.a);
            if (enemy.attackCooldown <= 0.f && (world.hasComponent<TowerComponent>(collisionEvent.b) || world.hasComponent<BaseComponent>(collisionEvent.b) ))
            {
                world.getComponent<HealthComponent>(collisionEvent.b).currentHP -= enemy.Damage;
                enemy.attackCooldown = 1.f / enemy.attackRate;
                SIBOLOG_DEBUG(std::format("Tower Damaged"));
            }
        }else if (world.hasComponent<EnemyComponent>(collisionEvent.b))
        {
            auto& enemy = world.getComponent<EnemyComponent>(collisionEvent.b);
            if (enemy.attackCooldown <= 0.f && (world.hasComponent<TowerComponent>(collisionEvent.a) || world.hasComponent<BaseComponent>(collisionEvent.a)))
            {
                world.getComponent<HealthComponent>(collisionEvent.a).currentHP -= enemy.Damage;
                enemy.attackCooldown = 1.f / enemy.attackRate;
                SIBOLOG_DEBUG(std::format("Tower Damaged"));
            }
        }
    }
}
