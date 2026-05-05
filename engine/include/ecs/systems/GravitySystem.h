#pragma once
#include "ecs/System.h"
#include "ecs/Types.h"
#include "ecs/World.h"
#include "ecs/components/GravityComponent.h"
#include "ecs/components/TransformComponent.h"

class GravitySystem : public System
{
public:
    GravitySystem(float screenWidth, float screenHeight) 
    : m_screenWidth(screenWidth), m_screenHeight(screenHeight)
    {}
    
    void update(World& world, float deltaTime) override
    {
        for (Entity e : world.getEntitiesWith<TransformComponent, GravityComponent>())
        {
            auto& t = world.getComponent<TransformComponent>(e);
            auto& g = world.getComponent<GravityComponent>(e);
        
            t.velY += g.force * deltaTime;
        }
    }
    
private:
    float m_screenWidth;
    float m_screenHeight;
};
