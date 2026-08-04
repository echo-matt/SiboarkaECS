#include "CollisionSystem.h"

#include <format>

#include "components/EnemyComponent.h"
#include "components/TowerComponent.h"
#include "ecs/Logger.h"
#include "ecs/components/ProjectileComponent.h"
#include "ecs/systems/CollisionEventSystem.h"

void CollisionSystem::update(World& world, float deltaTime)
{
    world.getEntitiesWith<TransformComponent, ColliderComponent>(entities);

    for (size_t i = 0; i < entities.size(); i++)
    {
        auto& t1 = world.getComponent<TransformComponent>(entities[i]);
        auto& c1 = world.getComponent<ColliderComponent>(entities[i]);
        bool s1 = world.hasComponent<StaticComponent>(entities[i]);
        
        for (size_t j = i + 1; j < entities.size(); j++)
        {
            bool s2 = world.hasComponent<StaticComponent>(entities[j]);
            
            if (s1 && s2) continue;

            auto& t2 = world.getComponent<TransformComponent>(entities[j]);
            auto& c2 = world.getComponent<ColliderComponent>(entities[j]);


            float overlapX = std::min(t1.x + c1.width, t2.x + c2.width) - std::max(t1.x, t2.x);
            float overlapY = std::min(t1.y + c1.height, t2.y + c2.height) - std::max(t1.y, t2.y);

            //If there is a collision
            if (overlapX > 0 && overlapY > 0)
            {
                world.events.emit(CollisionEvent{entities[i], entities[j]});
            }
        }
    }
}
