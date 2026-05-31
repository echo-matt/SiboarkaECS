#pragma once
#include <vector>

#include "ecs/System.h"
#include "ecs/Types.h"

class DeathSystem : public System
{
public:
    void update(World& world, float deltaTime) override;
    
private:
    std::vector<Entity> m_EntitiesToDelete;
};
