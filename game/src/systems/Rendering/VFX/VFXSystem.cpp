#include "VFXSystem.h"

#include "components/DeadComponent.h"
#include "components/LifetimeComponent.h"
#include "components/RenderComponent.h"
#include "components/VFX/LaserComponent.h"
#include "ecs/World.h"
#include "ecs/components/TransformComponent.h"
#include "events/ShotFiredEvent.h"

void VFXSystem::update(World& world, float deltaTime)
{
    for (ShotFiredEvent event : world.events.getEvents<ShotFiredEvent>())
    {
        
        float spawnX = world.getComponent<TransformComponent>(event.sourceEntity).x + world.getComponent<RenderComponent>(event.sourceEntity).width  / 2.f;
        float spawnY = world.getComponent<TransformComponent>(event.sourceEntity).y + world.getComponent<RenderComponent>(event.sourceEntity).height  / 2.f;
        
        if (world.isAlive(event.sourceEntity) && world.isAlive(event.targetEntity))
        {
            Entity laser = world.createEntity();
            world.addComponent(laser, LaserComponent{
            spawnX,
            spawnY,
            world.getComponent<TransformComponent>(event.targetEntity).x,
            world.getComponent<TransformComponent>(event.targetEntity).y, RED});
            world.addComponent(laser, LifetimeComponent{0.2f,0.2f});
        }
    }
    for (Entity e : world.getEntitiesWith<LifetimeComponent>())
    {
        world.getComponent<LifetimeComponent>(e).remaining -= deltaTime;
        if (world.getComponent<LifetimeComponent>(e).remaining <= 0)
        {
            world.addComponent(e, DeadComponent{});
        }
    }
}
