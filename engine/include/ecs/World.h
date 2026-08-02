#pragma once
#include "Types.h"

#include <unordered_map>
#include <typeindex>    
#include <any>          
#include <vector>       
#include <cassert>      
#include <algorithm>    
#include <unordered_set>

#include "EventBus.h"

class World {
public:
    Entity createEntity();

    void destroyEntity(Entity entity);
    
    bool isAlive(Entity entity) const;

    template<typename T>
    void addComponent(Entity entity, T component) {
        
        assert(isAlive(entity) && "Cannot add a component to a dead entity");
        
        auto key = std::type_index(typeid(T));

        m_components[key][entity] = std::move(component);
    }

    template<typename T>
    T& getComponent(Entity entity) {
        assert(isAlive(entity) && "Cannot get component from a dead entity");
        assert(hasComponent<T>(entity) && "Entity does not have the requested component!");
        
        auto key = std::type_index(typeid(T));
        return std::any_cast<T&>(m_components[key][entity]);
    }

    template<typename T>
    const T& getComponent(Entity entity) const {
        assert(isAlive(entity) && "Cannot get component from a dead entity");
        assert(hasComponent<T>(entity) && "Entity does not have the requested component!");
        
        auto key = std::type_index(typeid(T));
        return std::any_cast<const T&>(m_components.at(key).at(entity));
    }
    
    template<typename T>
    bool hasComponent(Entity entity) const {
        
        if (!isAlive(entity)) return false; // Cannot get a component from a dead entity
        
        auto key = std::type_index(typeid(T));
        
        auto typeIt = m_components.find(key);
        if (typeIt == m_components.end()) return false;

        return typeIt->second.count(entity) > 0;
    }

    template<typename T>
    void removeComponent(Entity entity) {
        
        if (!isAlive(entity)) return;   // Cannot remove a component froma dead entity
        auto key = std::type_index(typeid(T));
        auto typeIt = m_components.find(key);
        if (typeIt != m_components.end())
        {
            typeIt->second.erase(entity);
        }
    }

    template<typename... Ts>
    std::vector<Entity> getEntitiesWith() {
        std::vector<Entity> result;
        
        for (uint32_t index = 0; index < m_liveIndices.size(); ++index)
        {
            if (m_liveIndices[index])
            {
                Entity e = makeEntity(index, m_generations[index]);

                if ((hasComponent<Ts>(e) && ... ))
                {
                    result.push_back(e);
                }
            }
        }

        return result;
    }
    
    EventBus events;
private:
    
    uint32_t m_nextIndex = 1;            // next unused index
    std::vector<uint16_t> m_generations; //generation per index
    std::vector<uint32_t> m_freeIndices; //recycled indices
    std::vector<bool> m_liveIndices;  // alive entities

    std::unordered_map<std::type_index,std::unordered_map<Entity, std::any>> m_components;
    
};
