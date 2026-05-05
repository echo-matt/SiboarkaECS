#include "PlacementSystem.h"

#include <format>

#include "ShootingSystem.h"
#include "components/DeadComponent.h"
#include "components/HealthComponent.h"
#include "components/RenderComponent.h"
#include "components/TargetComponent.h"
#include "components/TowerComponent.h"
#include "ecs/Logger.h"
#include "ecs/World.h"
#include "ecs/components/ColliderComponent.h"
#include "ecs/components/TransformComponent.h"
#include "events/PlaceTowerRequestEvent.h"
#include "events/RemoveTowerRequestEvent.h"
#include "events/TowerPlacedEvent.h"

PlacementSystem::PlacementSystem(float screenW, float screenH, bool(&grid)[30][30]) : _grid(grid), _screenW(screenW), _screenH(screenH)
{
}

void PlacementSystem::update(World& world, float deltaTime)
{
    for (PlaceTowerRequestEvent e : world.events.getEvents<PlaceTowerRequestEvent>())
    {
        auto coords = e.coords;
        
        if (_grid[e.gridCell.x][e.gridCell.y] != false)
        {
            continue;
        }
        
        float cellW = (_screenW / 30.f);
        float cellH = (_screenH / 30.f);
        
        Entity placedTower = world.createEntity();
        world.addComponent(placedTower, TowerComponent{});
        world.addComponent(placedTower, TransformComponent{e.coords.x, e.coords.y, 0, 0});
        world.addComponent(placedTower, RenderComponent{cellW, cellH, PURPLE, 0, Box});
        world.addComponent(placedTower, ColliderComponent{cellW, cellH});
        world.addComponent(placedTower, ShootingSystem{});
        world.addComponent(placedTower, TargetComponent{std::pair<int,int>(e.coords.x, e.coords.y)});
        world.addComponent(placedTower, HealthComponent{1000, 1000});
        
        _grid[e.gridCell.x][e.gridCell.y] = true;
        
        world.events.emit(TowerPlacedEvent{placedTower});
        SIBOLOG_DEBUG(std::format("Placed tower at {}, {} - Grid cell {}, {}", e.coords.x, e.coords.y, e.gridCell.x, e.gridCell.y));
    }
    
    for (RemoveTowerRequestEvent e : world.events.getEvents<RemoveTowerRequestEvent>())
    {
        if (_grid[e.gridCell.x][e.gridCell.y] != false)
        {
            for (Entity tower : world.getEntitiesWith<TowerComponent>())
            {
                if (world.getComponent<TransformComponent>(tower).x == e.coords.x && world.getComponent<TransformComponent>(tower).y == e.coords.y)
                {
                    world.addComponent(tower, DeadComponent{});
                    SIBOLOG_DEBUG(std::format("Removed tower at {} {}", e.gridCell.x, e.gridCell.y));
                    _grid[e.gridCell.x][e.gridCell.y] = false;
                }
            }
        }
    }
}
