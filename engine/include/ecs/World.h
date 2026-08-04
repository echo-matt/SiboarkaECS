#pragma once
#include "Types.h"

#include <unordered_map>
#include <typeindex>    
#include <vector>       
#include <cassert>      
#include <algorithm>    
#include <array>
#include <memory>
#include <span>

#include "EventBus.h"
#include "storage/ComponentView.h"
#include "storage/IComponentStore.h"
#include "storage/SparseSet.h"

class World {
public:
    Entity createEntity();

    void destroyEntity(Entity entity);
    
    bool isAlive(Entity entity) const;

    template<typename T>
    void addComponent(Entity entity, T component) {
        
        assert(isAlive(entity) && "Cannot add a component to a dead entity");
        
        SparseSet<T>* store = getOrCreateStore<T>();
        
        store->insert(entity, std::move(component));
    }

    template<typename T>
    T& getComponent(Entity entity) {
        assert(isAlive(entity) && "Cannot get component from a dead entity");
        assert(hasComponent<T>(entity) && "Entity does not have the requested component!");
        
        SparseSet<T>* store = findStore<T>();
        assert(store != nullptr && "Store not valid");
        
        return store->get(entity);
    }

    template<typename T>
    const T& getComponent(Entity entity) const {
        assert(isAlive(entity) && "Cannot get component from a dead entity");
        assert(hasComponent<T>(entity) && "Entity does not have the requested component!");
        
        // store is declared as const because inside a const function it needs to be a non writable pointer.
        // so findStore<T> passes through the const version that returns a const, so the variable assigned to it
        // needs to be const too. The same happens in hasComponent below
        const SparseSet<T>* store = findStore<T>();
        assert(store != nullptr && "Store not valid");
        
        return store->get(entity);
    }
    
    template<typename T>
    std::span<T> getComponents()
    {
        SparseSet<T>* store = getOrCreateStore<T>();
        return store->data();
    }
    
    template<typename T>
    bool hasComponent(Entity entity) const {
        
        if (!isAlive(entity)) return false; // Cannot get a component from a dead entity
        
        const SparseSet<T>* store = findStore<T>();
        if (store == nullptr)
        {
            return false;
        }
        return store->has(entity);
    }

    template<typename T>
    void removeComponent(Entity entity) {
        
        if (!isAlive(entity)) return;   // Cannot remove a component froma dead entity
        
        SparseSet<T>* store = findStore<T>();
        if (store == nullptr)
        {
            return;
        }
        store->remove(entity);
    }

    // This function finds the entities with the requested components
    // It first gets the store with the rarest component
    // Then queries the entities inside that store to check if they have all the required components
    template<typename... Ts>
    std::vector<Entity> getEntitiesWith() {
        
        static_assert(sizeof ... (Ts) > 0, "getEntitiesWith called with no components");
        //Empty result vector
        std::vector<Entity> result;
        //Array of IComponentStore* size of how many components i am passing in
        std::array<IComponentStore*, sizeof...(Ts)> stores = { findStore<Ts>() ... };
        
        // Iterate all the stores, if any is nullptr, return the empty vector.
        // A missing store means zero entities can possibily have this combination
        for (IComponentStore* store : stores)
        {
            if (store == nullptr) return result;
        }
        
        // Find the store with the smallest entity count
        IComponentStore* smallestStore = stores[0];
        size_t minSize = smallestStore->size();
        
        for (size_t i = 1; i < stores.size(); ++i)
        {
            if (stores[i]->size() < minSize)
            {
                smallestStore = stores[i];
                minSize = stores[i]->size();
            }
        }
        
        // To not reallocate the vector
        result.reserve(minSize);
        
        // Loop over the smallest store's entities
        for (Entity e : smallestStore->entities())
        {
            // Test if the entity has ALL the requested components
            if ((hasComponent<Ts>(e) && ...))
            {
                result.push_back(e);
            }
        }
        
        return result;
    }
    
    template<typename... Ts>
    void getEntitiesWith(std::vector<Entity>& outResult)
    {
        outResult.clear();
        
        static_assert(sizeof ... (Ts) > 0, "getEntitiesWith called with no components");
        
        std::array<IComponentStore*, sizeof...(Ts)> stores = { findStore<Ts>() ... };

        for (IComponentStore* store : stores)
        {
            if (store == nullptr)
            {
                return; 
            }
        }

        IComponentStore* smallestStore = stores[0];
        size_t minSize = smallestStore->size();
        
        for (size_t i = 1; i < stores.size(); ++i)
        {
            if (stores[i]->size() < minSize)
            {
                smallestStore = stores[i];
                minSize = stores[i]->size();
            }
        }
        
        outResult.reserve(minSize);

        for (Entity e : smallestStore->entities())
        {
            if ((hasComponent<Ts>(e) && ...))
            {
                outResult.push_back(e);
            }
        }
    }
    
    template <typename T>
    ComponentView<T> view()
    {
        return getOrCreateStore<T>();
    }

    EventBus events;
private:
    
    uint32_t m_nextIndex = 1;            // next unused index
    std::vector<uint16_t> m_generations; //generation per index
    std::vector<uint32_t> m_freeIndices; //recycled indices
    std::vector<bool> m_liveIndices;  // alive entities
    
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStore>> m_stores;
    
    
    template <typename T>
    SparseSet<T>* getOrCreateStore()
    {
        auto key = std::type_index(typeid(T));
        auto it = m_stores.find(key);

        if (it == m_stores.end())
        {
            it = m_stores.emplace(key, std::make_unique<SparseSet<T>>()).first;
        }
        
        return static_cast<SparseSet<T>*>(it->second.get());
    }
    
    template <typename T>
    SparseSet<T>* findStore()
    {
        auto key = std::type_index(typeid(T));
        auto it = m_stores.find(key);
        
        if (it == m_stores.end())
        {
            return nullptr;
        }
        
        return static_cast<SparseSet<T>*>(it->second.get());
    }
    
    template <typename T>
    const SparseSet<T>* findStore() const
    {
        auto key = std::type_index(typeid(T));
        auto it = m_stores.find(key);
        
        if (it == m_stores.end())
        {
            return nullptr;
        }
        
        return static_cast<const SparseSet<T>*>(it->second.get());
    }
    
};
