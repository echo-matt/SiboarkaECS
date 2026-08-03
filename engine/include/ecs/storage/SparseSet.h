#pragma once
#include <cassert>
#include <vector>
#include "IComponentStore.h"

#include "ecs/Types.h"

template <typename T>
class SparseSet : public IComponentStore
{
private:
    std::vector<uint32_t> sparse;
    std::vector<Entity> dense;
    std::vector<T> components;
    
    void validate() const
    {
        assert(dense.size() == components.size() && "Dense and components size mismatch");
        for (size_t i = 0; i < dense.size(); ++i)
        {
            Entity e = dense[i];
            assert(sparse[entityIndex(e)] == i && "Sparse to dense mapping corrupted");
        }
    }

public:
    
    bool has(Entity e) const
    {
        uint32_t i = entityIndex(e);
        
        return i < sparse.size() && sparse[i] < dense.size() && dense[sparse[i]] == e;
    }
    
    void insert(Entity e, T c)
    {
        if (has(e))
        {
            components[sparse[entityIndex(e)]] = std::move(c);
            validate();
            return;
        }
        
        uint32_t index = entityIndex(e);

        if (index >= sparse.size())
        {
            sparse.resize(index + 1, index);
        }
        sparse[index] = static_cast<uint32_t>(dense.size());
        dense.push_back(e);
        components.push_back(std::move(c));
        
        validate();
    }
    
    T& get(Entity e)
    {
        assert(has(e) && "Cannot get component: Entity is not a member");
        return components[sparse[entityIndex(e)]];
    }
    
    const T& get(Entity e) const
    {
        assert(has(e) && "Cannot get component: Entity is not a member");
        return components[sparse[entityIndex(e)]];
    }
    
    void remove(Entity e) override
    {
        // If the entity does not exist
        if (!has(e)) return;
        
        uint32_t i = sparse[entityIndex(e)];
        Entity last = dense.back();
        // If the entity to remove IS NOT the last one
        if (i != dense.size() - 1)
        {
            dense[i] = last; // Overwrite the target slot with the last element
            components[i] = std::move(components.back()); // Move the components of the last entity into the overwritten entity
            sparse[entityIndex(last)] = i; // Update the sparse array with the new poisition of the entity
        }
        
        dense.pop_back(); // Pop the now duplicate entry / Last entry if the entity to remove IS the last
        components.pop_back();
        
        validate();
    }
    
    const std::vector<Entity>& entities() const override { return dense; }
    std::vector<T>& data() { return components; }
    const std::vector<T>& data() const { return components; }
    
    size_t size() const override { return dense.size(); }
};
