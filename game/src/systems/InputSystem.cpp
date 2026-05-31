#include "InputSystem.h"

InputSystem::InputSystem(float screenW, float screenH)
{
    m_screenW = screenW;
    m_screenH = screenH;
}


void InputSystem::update(World& world, float deltaTime)
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
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_inputState.bMouseInViewport)
        {
                
            Vector2 mousePos = {m_inputState.mouseX, m_inputState.mouseY};
            auto cellWidth = m_screenW / 30;
            auto cellHeight = m_screenH / 30;
            int cellCol = (int)(mousePos.x/ cellWidth);
            int cellRow = (int)(mousePos.y / cellHeight);
            float snappedX = cellCol * cellWidth;
            float snappedY = cellRow * cellHeight;
            if (cellCol >= 0 && cellRow >= 0 && cellCol < 30 && cellRow < 30)
            {
                world.events.emit(PlaceTowerRequestEvent{cellCol, cellRow, snappedX, snappedY});
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && m_inputState.bMouseInViewport)
        {
            Vector2 mousePos = {m_inputState.mouseX, m_inputState.mouseY};
            auto cellWidth = m_screenW / 30;
            auto cellHeight = m_screenH / 30;
            int cellCol = (int)(mousePos.x / cellWidth);
            int cellRow = (int)(mousePos.y / cellHeight);
            float snappedX = cellCol * cellWidth;
            float snappedY = cellRow * cellHeight;
            
            world.events.emit(RemoveTowerRequestEvent{cellCol, cellRow, snappedX, snappedY});
            SIBOLOG_DEBUG(std::format("Right click at: {},{} " ,cellCol, cellRow));
            //TODO RIMUOVI BENE LE TOWER CHE I NEMICI CI VANNO ANCORA
        }
    }

void InputSystem::setInputState(const InputState& inputState)
{
    m_inputState = inputState;
}
