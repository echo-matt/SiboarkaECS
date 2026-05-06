#pragma once
#include <format>
#include <ecs/System.h>
#include <ecs/World.h>
#include <ecs/components/TransformComponent.h>

#include "components/EnemyComponent.h"
#include "ecs/Logger.h"
#include "ecs/components/GravityComponent.h"

class MovementSystem : public System
{
public:

    MovementSystem(float screenWidth, float screenHeight)
        : m_screenWidth(screenWidth), m_screenHeight(screenHeight)
    {}

    void update(World& world, float deltaTime) override
    {

        for (Entity e : world.getEntitiesWith<TransformComponent>())
        {
            auto& t = world.getComponent<TransformComponent>(e);
            float oldX = t.x;
            float oldY = t.y;
            t.x += t.velX * deltaTime;
            t.y += t.velY * deltaTime;
        }
    }
private:
    float m_screenWidth;
    float m_screenHeight;
};
