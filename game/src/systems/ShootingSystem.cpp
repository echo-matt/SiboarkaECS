#include "ShootingSystem.h"

#include "components/BulletComponent.h"
#include "components/EnemyComponent.h"
#include "components/RenderComponent.h"
#include "components/TowerComponent.h"
#include "ecs/Types.h"
#include "ecs/World.h"
#include "ecs/components/ColliderComponent.h"
#include "ecs/components/ProjectileComponent.h"
#include "ecs/components/TransformComponent.h"

void ShootingSystem::update(World& world, float deltaTime)
{
    const auto& enemies = world.getEntitiesWith<EnemyComponent, TransformComponent>();
    for (Entity e : world.getEntitiesWith<TowerComponent, TransformComponent>())
    {
        TowerComponent& towerComponent = world.getComponent<TowerComponent>(e);
        TransformComponent& towerTransform = world.getComponent<TransformComponent>(e);
        
        towerComponent.cooldownTimer -= deltaTime;
        if (towerComponent.cooldownTimer > 0.0f) continue;
        
        float minDist = std::numeric_limits<float>::max();
        
        float rangeSq = towerComponent.range * towerComponent.range;
        
        Entity closestEnemy = NULL_ENTITY;
        
        for (Entity enemy : enemies)
        {
            const auto& enemyTransform = world.getComponent<TransformComponent>(enemy);
            
            const float dx = enemyTransform.x - towerTransform.x;
            const float dy = enemyTransform.y - towerTransform.y;
            const float distSq = dx * dx + dy * dy;
            
            if (distSq > rangeSq) continue;
            
            if (distSq < minDist)
            {
                minDist = distSq;
                closestEnemy = enemy;
            }
        }
        
        if (closestEnemy != NULL_ENTITY)
        {
            Entity bullet = world.createEntity();
            world.addComponent(bullet, TransformComponent{towerTransform.x, towerTransform.y, 150.f, 150.f});
            world.addComponent(bullet, BulletComponent{});
            world.addComponent(bullet, ProjectileComponent{});
            world.addComponent(bullet, RenderComponent{2, 2});
            world.addComponent(bullet, ColliderComponent{0.1f, 0.1f});
            towerComponent.cooldownTimer = 1.0f / towerComponent.fireRate;
            
            const auto& enemyTransform = world.getComponent<TransformComponent>(closestEnemy);
            auto& bulletTransform = world.getComponent<TransformComponent>(bullet);
            
            auto dx = enemyTransform.x - bulletTransform.x;
            auto dy = enemyTransform.y - bulletTransform.y;
            
            auto length = sqrt(dx*dx + dy*dy);
            
            auto dirX = dx / length;
            auto dirY = dy / length;
            
            bulletTransform.velX = dirX * bulletTransform.velX;
            bulletTransform.velY = dirY * bulletTransform.velY;
            
        }
        
    }
}
