#include "DamageSystem.h"

#include <format>

#include "components/BulletComponent.h"
#include "components/DeadComponent.h"
#include "components/EnemyComponent.h"
#include "components/HealthComponent.h"
#include "components/TowerComponent.h"
#include "ecs/Logger.h"
#include "ecs/World.h"
#include "ecs/systems/CollisionEventSystem.h"

void DamageSystem::update(World& world, float deltaTime)
{
    auto& collisionEvents = world.events.getEvents<CollisionEvent>();
    
    for (const auto& collisionEvent : collisionEvents)
    {
        /*if (world.hasComponent<BulletComponent>(collisionEvent.a))
        {
            if (world.hasComponent<HealthComponent>(collisionEvent.b))
            {
                world.getComponent<HealthComponent>(collisionEvent.b).currentHP -= world.getComponent<BulletComponent>(collisionEvent.a).Damage;
                world.addComponent(collisionEvent.a, DeadComponent{});
                SIBOLOG_DEBUG(std::format("Damage"));
            }
        }else if (world.hasComponent<BulletComponent>(collisionEvent.b))
        {
            if (world.hasComponent<HealthComponent>(collisionEvent.a))
            {
                world.getComponent<HealthComponent>(collisionEvent.a).currentHP -= world.getComponent<BulletComponent>(collisionEvent.b).Damage;
                world.addComponent(collisionEvent.b, DeadComponent{});
                SIBOLOG_DEBUG(std::format("Damage"));
            }
        }*/

        if (world.hasComponent<EnemyComponent>(collisionEvent.a))
        {
            if (world.hasComponent<TowerComponent>(collisionEvent.b))
            {
                world.getComponent<HealthComponent>(collisionEvent.b).currentHP -= world.getComponent<EnemyComponent>(collisionEvent.a).Damage;
                SIBOLOG_DEBUG(std::format("Tower Damaged"));
            }
        }else if (world.hasComponent<EnemyComponent>(collisionEvent.b))
        {
            if (world.hasComponent<TowerComponent>(collisionEvent.a))
            {
                world.getComponent<HealthComponent>(collisionEvent.a).currentHP -= world.getComponent<EnemyComponent>(collisionEvent.b).Damage;
                SIBOLOG_DEBUG(std::format("Tower Damaged"));

            }
        }
    }
}
