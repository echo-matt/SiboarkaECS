#pragma once

class World;

class Scene
{
public:
    virtual ~Scene() = default;
    
    virtual void load(World& world) = 0;
    virtual void update(World& world, float deltaTime) = 0;
    virtual void unload(World& world) = 0;
    
};