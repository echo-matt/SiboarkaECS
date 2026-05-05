#pragma once

#include <ecs/System.h>
#include <ecs/World.h>
#include <ecs/components/TransformComponent.h>
#include <ecs/EventBus.h>
#include <ecs/events/CollisionDirection.h>

#include "CollisionEventSystem.h"
#include "ecs/components/ColliderComponent.h"
#include "ecs/components/GroundedComponent.h"
#include "ecs/components/StaticComponent.h"

class CollisionSystem : public System
{
public:
    void update(World& world, float deltaTime) override;
};
