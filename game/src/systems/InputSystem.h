#pragma once
#include <format>
#include <ecs/System.h>
#include <ecs/World.h>
#include <ecs/components/TransformComponent.h>
#include <ecs/components/ControllableComponent.h>
#include <raylib.h>

#include "ecs/Logger.h"
#include "ecs/components/GroundedComponent.h"
#include "events/PlaceTowerRequestEvent.h"
#include "events/RemoveTowerRequestEvent.h"

class InputSystem : public System
{
public:
    
    InputSystem(float screenW, float screenH);
    void update(World& world, float deltaTime) override
    {
        for (Entity e : world.getEntitiesWith<TransformComponent, ControllableComponent>())
        {
            auto& t = world.getComponent<TransformComponent>(e);
            
            t.velX = 0.f;
            t.velY = 0.f;
            
            if (IsKeyDown(KEY_W)) t.velY = -250.f;
            if (IsKeyDown(KEY_S)) t.velY =  250.f;
            if (IsKeyDown(KEY_A)) t.velX = -250.f;
            if (IsKeyDown(KEY_D)) t.velX =  250.f;
            if (IsKeyPressed(KEY_SPACE) && world.hasComponent<GroundedComponent>(e))
            {
                world.removeComponent<GroundedComponent>(e);
                t.velY += -450.f;
            }
        }
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
                
            auto mousePos = GetMousePosition();
            auto cellWidth = _screenW / 30;
            auto cellHeight = _screenH / 30;
            int cellCol = (int)(mousePos.x / cellWidth);
            int cellRow = (int)(mousePos.y / cellHeight);
            float snappedX = cellCol * cellWidth;
            float snappedY = cellRow * cellHeight;
            if (cellCol < 30 && cellRow < 30)
            {
                world.events.emit(PlaceTowerRequestEvent{cellCol, cellRow, snappedX, snappedY});
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            auto mousePos = GetMousePosition();
            auto cellWidth = _screenW / 30;
            auto cellHeight = _screenH / 30;
            int cellCol = (int)(mousePos.x / cellWidth);
            int cellRow = (int)(mousePos.y / cellHeight);
            float snappedX = cellCol * cellWidth;
            float snappedY = cellRow * cellHeight;
            
            world.events.emit(RemoveTowerRequestEvent{cellCol, cellRow, snappedX, snappedY});
            SIBOLOG_DEBUG(std::format("Right click at: {},{} " ,cellCol, cellRow));
        }
    }
    
private:
    float _screenW,_screenH;
};

inline InputSystem::InputSystem(float screenW, float screenH)
{
    _screenH = screenH;
    _screenW = screenW;
}
