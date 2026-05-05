#include <ecs/systems/CollisionSystem.h>

#include <format>

#include "../../../../game/src/components/EnemyComponent.h"
#include "ecs/Logger.h"
#include "ecs/components/ProjectileComponent.h"

void CollisionSystem::update(World& world, float deltaTime)
{
    auto entities = world.getEntitiesWith<TransformComponent, ColliderComponent>();

    for (size_t i = 0; i < entities.size(); i++)
    {
        for (size_t j = i + 1; j < entities.size(); j++)
        {
            if (i == j) continue;

            auto& t1 = world.getComponent<TransformComponent>(entities[i]);
            auto& t2 = world.getComponent<TransformComponent>(entities[j]);
            auto& c1 = world.getComponent<ColliderComponent>(entities[i]);
            auto& c2 = world.getComponent<ColliderComponent>(entities[j]);
            bool s1 = world.hasComponent<StaticComponent>(entities[i]);
            bool s2 = world.hasComponent<StaticComponent>(entities[j]);

            if (s1 && s2) continue;


            float overlapX = std::min(t1.x + c1.width, t2.x + c2.width) - std::max(t1.x, t2.x);
            float overlapY = std::min(t1.y + c1.height, t2.y + c2.height) - std::max(t1.y, t2.y);

            //If there is a collision
            if (overlapX > 0 && overlapY > 0)
            {
                if (world.hasComponent<ProjectileComponent>(entities[i]) || world.hasComponent<ProjectileComponent>(entities[j]))
                {
                    world.events.emit(CollisionEvent{entities[i], entities[j]});
                    //SIBOLOG_DEBUG(std::format("Collision detected between {} and {}", entities[i], entities[j]));
                }
                
                if (world.hasComponent<EnemyComponent>(entities[i]) || world.hasComponent<EnemyComponent>(entities[j]))
                {
                    world.events.emit(CollisionEvent{entities[i], entities[j]});
                    SIBOLOG_DEBUG(std::format("Collision detected between {} and {}", entities[i], entities[j]));
                }
            }
        }
    }
}
