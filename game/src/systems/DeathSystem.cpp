#include "DeathSystem.h"

#include "components/DeadComponent.h"
#include "components/HealthComponent.h"
#include "ecs/World.h"
#include "events/DeathEvent.h"

void DeathSystem::update(World& world, float deltaTime)
{
    for (Entity e : world.getEntitiesWith<HealthComponent>())
    {
        if (world.getComponent<HealthComponent>(e).currentHP <= 0)
        {
            world.addComponent(e, DeadComponent{});
            world.events.emit(DeathEvent{e});
        }
    }
    
    for (Entity e : world.getEntitiesWith<DeadComponent>())
    {
        world.destroyEntity(e);
    }
}
