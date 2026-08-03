#pragma once
#include <vector>

#include "ecs/Types.h"

class IComponentStore
{
public:
    virtual ~IComponentStore() = default;
    
    virtual void remove(Entity entity) = 0;
    
    virtual size_t size() const = 0;
    
    virtual const std::vector<Entity>& entities() const = 0;
};
