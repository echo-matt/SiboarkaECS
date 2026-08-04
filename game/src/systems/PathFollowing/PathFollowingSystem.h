#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ecs/System.h"
#include "ecs/Types.h"

class PathFollowingSystem : public System
{
public:
    PathFollowingSystem();
    void update(World& world, float deltaTime) override;
private:
    std::unordered_map<Entity, std::pair<int,int>> m_waypoints;
    std::vector<Entity> m_queryBuffer;
    std::unordered_set<Entity> m_blockedEnemies;

};
