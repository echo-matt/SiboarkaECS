#include "PathFollowingSystem.h"

#include <format>
#include <limits>

#include "components/BaseComponent.h"
#include "components/EnemyComponent.h"
#include "components/TargetComponent.h"
#include "components/TowerComponent.h"
#include "ecs/Types.h"
#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/systems/CollisionEventSystem.h"
#include "events/DeathEvent.h"
#include "events/TowerPlacedEvent.h"

PathFollowingSystem::PathFollowingSystem()
{
}

void PathFollowingSystem::update(World& world, float deltaTime)
{
    m_blockedEnemies.clear();
    
    world.getEntitiesWith<BaseComponent>(m_queryBuffer);
    for (Entity e : m_queryBuffer)
    {
        m_waypoints.try_emplace(e, world.getComponent<TargetComponent>(e).TargetPosition);
    }
    for (const TowerPlacedEvent& event : world.events.getEvents<TowerPlacedEvent>())
    {
        m_waypoints.try_emplace(event.entity, world.getComponent<TargetComponent>(event.entity).TargetPosition);
    }
    
    for (const CollisionEvent& collisionEvent : world.events.getEvents<CollisionEvent>())
    {
        if (world.hasComponent<EnemyComponent>(collisionEvent.a))
        {
            if (world.hasComponent<TowerComponent>(collisionEvent.b) || world.hasComponent<BaseComponent>(collisionEvent.b))
            {
                m_blockedEnemies.insert(collisionEvent.a);
            }
        }else if (world.hasComponent<EnemyComponent>(collisionEvent.b))
        {
            if (world.hasComponent<TowerComponent>(collisionEvent.a) || world.hasComponent<BaseComponent>(collisionEvent.a))
            {
                m_blockedEnemies.insert(collisionEvent.b);
            }
        }
    }

    // --- ENEMY MOVEMENT LOOP ---
    world.getEntitiesWith<TransformComponent, EnemyComponent>(m_queryBuffer);
    for (Entity e : m_queryBuffer)
    {
        EnemyComponent& enemy_component = world.getComponent<EnemyComponent>(e);
        TransformComponent& transform_component = world.getComponent<TransformComponent>(e);

        float closestDist = std::numeric_limits<float>::max();
        Entity closestEntity = NULL_ENTITY;
        std::pair<int, int> closestWaypoint = {0,0};
        
        if (m_blockedEnemies.contains(e))
        {
            transform_component.velX = 0;
            transform_component.velY = 0;
        }else
        {
            for (const auto& [entity, waypoint] : m_waypoints)
            {
                auto dx = waypoint.first  - transform_component.x;
                auto dy = waypoint.second - transform_component.y;
                float dist = sqrt(dx * dx + dy * dy);
            
                if (dist < closestDist)
                {
                    closestDist = dist;
                    closestEntity = entity;
                    closestWaypoint = waypoint;
                }
            }
        
            if (closestEntity == NULL_ENTITY)
            {
                transform_component.velX = 0.f;
                transform_component.velY = 0.f;
                continue;
            }
            
            auto dx = closestWaypoint.first  - transform_component.x;
            auto dy = closestWaypoint.second - transform_component.y;
        
            float length = closestDist;

            auto dirX = dx / length;
            auto dirY = dy / length;

            transform_component.velX = dirX * enemy_component.speed;
            transform_component.velY = dirY * enemy_component.speed;
        }
    }
    
    for (const DeathEvent& event : world.events.getEvents<DeathEvent>())
    {
        m_waypoints.erase(event.entity);
    }
}
