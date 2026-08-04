#include <ecs/World.h>

#include "ecs/Logger.h"

Entity World::createEntity()
{
    uint32_t index;
    
    // Check the free list
    if (!m_freeIndices.empty())
    {
        index = m_freeIndices.back();   // Get the last element
        m_freeIndices.pop_back();       // Remove it
    }else
    {
        // Generate a new index
        if (m_nextIndex >= MAX_ENTITIES)
        {
            assert(false && "Maximim entity count reached"); // Assert for max entities reached
        }
        index = m_nextIndex++;
        
        // Grow the generations array to match
        if (index >= m_generations.size())
        {
            m_generations.resize(index + 1, 1);
        }
    }
    
    // Mark the index as live
    if (index >= m_liveIndices.size())
    {
        m_liveIndices.resize(index + 1, false);
    }
    
    m_liveIndices[index] = true;
    
    
    uint32_t generation = m_generations[index];
    
    return makeEntity(index, generation);
}

void World::destroyEntity(Entity entity) {
    
    // Double destroy is a no op
    if (!isAlive(entity)) return;
    
    // remove the entity components
    for (auto& [typeKey, store] : m_stores) {
        store->remove(entity);
    }
    
    uint32_t index = entityIndex(entity);
    //Increment generation to invalidate all existing handles
    ++m_generations[index];
    // Mark the index as non live
    m_liveIndices[index] = false;
    // Push the index in the free list for reuse
    m_freeIndices.push_back(index);
}

bool World::isAlive(Entity entity) const {
    uint32_t index = entityIndex(entity);
    uint32_t generation = entityGeneration(entity);
    
    // First check the bounds to avoid crashing from garbage handles
    if (index >= m_generations.size())
    {
        return false;
    }
    // Then check if the index is currently marked as live and check if the generation matches
    return m_liveIndices[index] && (m_generations[index] == generation);
}
