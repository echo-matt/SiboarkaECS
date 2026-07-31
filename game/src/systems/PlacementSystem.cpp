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
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "events/PlaceTowerRequestEvent.h"
#include "events/RemoveTowerRequestEvent.h"
#include "events/TowerPlacedEvent.h"

PlacementSystem::PlacementSystem(float screenW, float screenH, Entity(&grid)[30][30]) : _grid(grid), _screenW(screenW), _screenH(screenH)
{
}

void PlacementSystem::update(World& world, float deltaTime)
{
    for (PlaceTowerRequestEvent e : world.events.getEvents<PlaceTowerRequestEvent>())
    {
        if (_grid[e.gridCell.x][e.gridCell.y] != NULL_ENTITY)
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
        world.addComponent(placedTower, TagComponent{"Tower"});
        
        world.addComponent(placedTower, TargetComponent{std::pair<int,int>(
            (int)(e.coords.x + cellW / 2.f),
            (int)(e.coords.y + cellH / 2.f)
        )});
        world.addComponent(placedTower, HealthComponent{200, 200});
        
        _grid[e.gridCell.x][e.gridCell.y] = placedTower;
        
        world.events.emit(TowerPlacedEvent{placedTower});
        SIBOLOG_DEBUG(std::format("Placed tower at {}, {} - Grid cell {}, {}", e.coords.x, e.coords.y, e.gridCell.x, e.gridCell.y));
    }
    
    for (RemoveTowerRequestEvent e : world.events.getEvents<RemoveTowerRequestEvent>())
    {
        if (_grid[e.gridCell.x][e.gridCell.y] != NULL_ENTITY)
        {
            world.addComponent(_grid[e.gridCell.x][e.gridCell.y], DeadComponent{});
            _grid[e.gridCell.x][e.gridCell.y] = NULL_ENTITY;
            SIBOLOG_DEBUG(std::format("Removed tower at {} {}", e.gridCell.x, e.gridCell.y));
        }
    }
}
