#pragma once
#include "CollisionEventSystem.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include "ecs/components/GroundedComponent.h"
#include "ecs/components/TransformComponent.h"

class PhysicsResponseSystem : System
{
public:
    
    void update(World& world, float deltaTime)
    {
        for (auto& event : world.events.getEvents<CollisionEvent>())
        {
            if (event.direction == CollisionDirection::Bottom)
            {
                auto& t = world.getComponent<TransformComponent>(event.a);
                t.velY = 0.f;
                if (!world.hasComponent<GroundedComponent>(event.a))
                {
                    world.addComponent(event.a, GroundedComponent{});
                    printf("Grounded added\n");
                }
            }
        }
    }
};
