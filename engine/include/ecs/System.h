#pragma once

class World;

class System {
public:
    virtual ~System() = default;

    virtual void update(World& world, float deltaTime) = 0;
};
