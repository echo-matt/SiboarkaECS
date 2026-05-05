#pragma once
#include <memory>

#include "Scene.h"

class SceneManager
{
public:
    void start(std::unique_ptr<Scene> scene, World& world);
    void update(World& world, float deltaTime);
    void requestSwitch(std::unique_ptr<Scene> nextScene);
    void applyPendingTransition(World& world);
    
private:
    std::unique_ptr<Scene> _current;
    std::unique_ptr<Scene> _pending;
};
