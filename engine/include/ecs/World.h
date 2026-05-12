#pragma once
#include "Types.h"

#include <unordered_map>
#include <typeindex>    
#include <any>          
#include <vector>       
#include <cassert>      
#include <algorithm>    

#include "EventBus.h"

struct InputState
{
    float mouseX, mouseY = 0;
    bool imguiCapturingMouse = false;
    bool bMouseInViewport = false;
};
class World {
public:
    Entity createEntity();

    void destroyEntity(Entity entity);
    
    bool isAlive(Entity entity) const;

    template<typename T>
    void addComponent(Entity entity, T component) {
        
        auto key = std::type_index(typeid(T));

        m_components[key][entity] = std::move(component);
    }

    template<typename T>
    T& getComponent(Entity entity) {
        auto key = std::type_index(typeid(T));

        assert(hasComponent<T>(entity) && "Entity does not have the requested component!");

        return std::any_cast<T&>(m_components[key][entity]);
    }

    template<typename T>
    const T& getComponent(Entity entity) const {
        auto key = std::type_index(typeid(T));
        assert(hasComponent<T>(entity) && "Entity does not have the requested component!");
        return std::any_cast<const T&>(m_components.at(key).at(entity));
    }
    
    template<typename T>
    bool hasComponent(Entity entity) const {
        auto key = std::type_index(typeid(T));
        
        auto typeIt = m_components.find(key);
        if (typeIt == m_components.end()) return false;

        return typeIt->second.count(entity) > 0;
    }

    template<typename T>
    void removeComponent(Entity entity) {
        auto key = std::type_index(typeid(T));
        m_components[key].erase(entity);
    }

    template<typename... Ts>
    std::vector<Entity> getEntitiesWith() {
        std::vector<Entity> result;

        for (Entity e : m_aliveEntities) {

            if ((hasComponent<Ts>(e) && ...)) {
                result.push_back(e);
            }
        }

        return result;
    }
    
    EventBus events;
    InputState input = {};
private:
    Entity m_nextEntityID = 1;

    std::vector<Entity> m_aliveEntities;

    std::unordered_map<
        std::type_index,
        std::unordered_map<Entity, std::any>
    > m_components;
    
    
};
