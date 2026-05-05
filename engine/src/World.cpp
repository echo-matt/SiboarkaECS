#include <ecs/World.h>

#include "ecs/Logger.h"

Entity World::createEntity() {

    Entity id = m_nextEntityID++;

    m_aliveEntities.push_back(id);
    
    return id;
}

void World::destroyEntity(Entity entity) {

    for (auto& [typeKey, entityMap] : m_components) {

        entityMap.erase(entity);
    }

    m_aliveEntities.erase(
        std::remove(m_aliveEntities.begin(), m_aliveEntities.end(), entity),
        m_aliveEntities.end()
    );
}

bool World::isAlive(Entity entity) const {

    for (Entity e : m_aliveEntities) {
        if (e == entity) return true;
    }
    return false;
}
