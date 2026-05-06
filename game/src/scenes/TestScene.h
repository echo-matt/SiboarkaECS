#pragma once
#include "ecs/Scene.h"
#include "ecs/Types.h"
#include "systems/CollisionSystem.h"
#include "ecs/systems/GravitySystem.h"
#include "ecs/systems/MovementSystem.h"
#include "ecs/systems/PhysicsResponseSystem.h"
#include "systems/DamageSystem.h"
#include "systems/DeathSystem.h"
#include "systems/InputSystem.h"
#include "systems/PathFollowingSystem.h"
#include "systems/PlacementSystem.h"
#include "systems/RenderSystem.h"
#include "systems/ShootingSystem.h"
#include "systems/VFXSystem.h"
#include "systems/WaveSpawnerSystem.h"


class TestScene : Scene
{
public:
    TestScene(float screenW, float screenH);
    void load(World& world) override;
    void update(World& world, float deltaTime) override;
    void unload(World& world) override;

private:
    float _screenW, _screenH;
    std::vector<Entity> _entities;
    
    bool _grid[30][30] = {};
    
    InputSystem inputSys;
    MovementSystem movementSys;
    CollisionSystem collisionSys;
    PhysicsResponseSystem physicsResponseSys;
    RenderSystem renderSys;
    ShootingSystem shootingSys;
    WaveSpawnerSystem waveSpawnerSys;
    PlacementSystem placementSys;
    DamageSystem damageSys;
    DeathSystem deathSys;
    PathFollowingSystem pathSys;
    VFXSystem vfxSys;

};
