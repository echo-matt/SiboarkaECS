#include <ecs/World.h>

#include "ecs/Logger.h"

Entity World::createEntity() {

    Entity id = m_nextEntityID++;

    m_aliveEntities.insert(id);
    
    return id;
}

void World::destroyEntity(Entity entity) {

    for (auto& [typeKey, entityMap] : m_components) {
        entityMap.erase(entity);
    }
    m_aliveEntities.erase(entity);
}

bool World::isAlive(Entity entity) const {
    // O(1) lookup  with unordered_set instead of O(n)
    return m_aliveEntities.contains(entity);
}
