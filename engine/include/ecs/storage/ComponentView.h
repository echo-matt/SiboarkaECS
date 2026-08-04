#pragma once

#include "SparseSet.h"
#include "ecs/Types.h"
#include <utility>

template <typename T>
class ComponentView
{
    
private:
    SparseSet<T>* m_store;
    
public:
    
    ComponentView(SparseSet<T>* store) : m_store(store) {}
    
    class Iterator
    {
    public:
        Iterator(SparseSet<T>* store, size_t index)
            : m_store(store),
              m_index(index)
        {
        }
        
        std::pair<Entity, T&> operator*() const
        {
            return {m_store->entities()[m_index], m_store->data()[m_index]};
        }
        
        Iterator& operator++()
        {
            ++m_index;
            return *this;
        }
        
        bool operator!=(const Iterator& other) const
        {
            return m_index != other.m_index;
        }

    private:
        SparseSet<T>* m_store;
        size_t m_index;
        
    };
    
    Iterator begin() const
    {
        return Iterator(m_store, 0);
    }
        
    Iterator end() const
    {
        return Iterator(m_store, m_store->size());
    }
};

