#pragma once
#include <format>
#include <ecs/System.h>
#include <ecs/World.h>
#include <ecs/components/TransformComponent.h>
#include <ecs/components/ControllableComponent.h>
#include <raylib.h>

#include "InputState.h"
#include "ecs/Logger.h"
#include "ecs/components/GroundedComponent.h"
#include "events/PlaceTowerRequestEvent.h"
#include "events/RemoveTowerRequestEvent.h"

class InputSystem : public System
{
public:
    
    InputSystem(float screenW, float screenH);
    void update(World& world, float deltaTime) override;
    void setInputState(const InputState& inputState);
    
private:
    float m_screenW,m_screenH;
    InputState m_inputState;
};