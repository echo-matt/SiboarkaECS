#pragma once
#include "ecs/Scene.h"
#include "ecs/Types.h"
#include "systems/Collision/CollisionSystem.h"
#include "ecs/systems/MovementSystem.h"
#include "ecs/systems/PhysicsResponseSystem.h"
#include "systems/Damage/DamageSystem.h"
#include "systems/Death/DeathSystem.h"
#include "systems/GameState/GameStateSystem.h"
#include "systems/Input/InputSystem.h"
#include "systems/PathFollowing/PathFollowingSystem.h"
#include "systems/Placement/PlacementSystem.h"
#include "systems/Rendering/RenderSystem.h"
#include "systems/Rendering/UI/UI.h"
#include "systems/Shooting/ShootingSystem.h"
#include "systems/Rendering/VFX/VFXSystem.h"
#include "systems/WaveSpawner/WaveSpawnerSystem.h"


class TestScene : Scene
{
public:
    TestScene(float screenW, float screenH);
    void load(World& world) override;
    void update(World& world, float deltaTime) override;
    void unload(World& world) override;
    
    //
    void setInputState(const InputState& state);
    void DrawGameOver();

private:
    float _screenW, _screenH;
    std::vector<Entity> _entities;
    
    Entity _grid[30][30] = {};
    
    GameStateSystem gameStateSys;
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
    UI uiSys;

};
