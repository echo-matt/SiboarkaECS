#include "DeathSystem.h"

#include <format>

#include "components/DeadComponent.h"
#include "components/HealthComponent.h"
#include "ecs/Logger.h"
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
            SIBOLOG_DEBUG(std::format("Entity death: {}", e));
        }
    }
    
    for (Entity e : world.getEntitiesWith<DeadComponent>())
    {
        world.destroyEntity(e);
    }
}
