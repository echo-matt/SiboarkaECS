#pragma once
#include <unordered_map>
#include <vector>

#include "ecs/System.h"
#include "ecs/Types.h"

class PathFollowingSystem : public System
{
public:
    PathFollowingSystem();
    void update(World& world, float deltaTime) override;
private:
    std::unordered_map<Entity, std::pair<int,int>> _waypoints;
};
