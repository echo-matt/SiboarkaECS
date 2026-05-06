#include "CollisionSystem.h"

#include <format>

#include "components/EnemyComponent.h"
#include "components/TowerComponent.h"
#include "ecs/Logger.h"
#include "ecs/components/ProjectileComponent.h"
#include "ecs/systems/CollisionEventSystem.h"

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
                world.events.emit(CollisionEvent{entities[i], entities[j]});
            }
        }
    }
}
