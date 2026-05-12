#include "PathFollowingSystem.h"

#include <algorithm>
#include <format>
#include <limits>

#include "components/BaseComponent.h"
#include "components/EnemyComponent.h"
#include "components/TargetComponent.h"
#include "ecs/Logger.h"
#include "ecs/Types.h"
#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"
#include "events/DeathEvent.h"
#include "events/TowerPlacedEvent.h"

PathFollowingSystem::PathFollowingSystem()
{
}

void PathFollowingSystem::update(World& world, float deltaTime)
{
    //TODO There is a bug where if an enemy has reached a target and a new one gets placed and then immediately removed, the enemy stays still and does not move towards the old target
    for (Entity e : world.getEntitiesWith<BaseComponent>())
    {
        _waypoints.try_emplace(e, world.getComponent<TargetComponent>(e).TargetPosition);
    }
    for (const TowerPlacedEvent& event : world.events.getEvents<TowerPlacedEvent>())
    {
        _waypoints.try_emplace(event.entity, world.getComponent<TargetComponent>(event.entity).TargetPosition);
    }

    // --- ENEMY MOVEMENT LOOP ---
    for (Entity e : world.getEntitiesWith<TransformComponent, EnemyComponent>())
    {
        EnemyComponent& enemy_component = world.getComponent<EnemyComponent>(e);
        TransformComponent& transform_component = world.getComponent<TransformComponent>(e);

        float closestDist = std::numeric_limits<float>::max();
        Entity closestEntity = NULL_ENTITY;

        for (const auto& [entity, waypoint] : _waypoints)
        {
            if (enemy_component.visitedTargets.contains(entity)) continue;
            
            auto dx = waypoint.first  - transform_component.x;
            auto dy = waypoint.second - transform_component.y;
            float dist = sqrt(dx * dx + dy * dy);
            
            if (dist < closestDist)
            {
                closestDist = dist;
                closestEntity = entity;
            }
        }
        
        if (closestEntity == NULL_ENTITY)
        {
            transform_component.velX = 0.f;
            transform_component.velY = 0.f;
            continue;
        }
        
        auto& targetPos = _waypoints[closestEntity];
        auto dx = targetPos.first  - transform_component.x;
        auto dy = targetPos.second - transform_component.y;
        
        float length = closestDist;

        auto dirX = dx / length;
        auto dirY = dy / length;

        transform_component.velX = dirX * enemy_component.speed;
        transform_component.velY = dirY * enemy_component.speed;

        if (length < 5.f)
        {
            enemy_component.visitedTargets.insert(closestEntity);
        }
    }
    
    for (const DeathEvent& event : world.events.getEvents<DeathEvent>())
    {
        _waypoints.erase(event.entity);
    }
}
