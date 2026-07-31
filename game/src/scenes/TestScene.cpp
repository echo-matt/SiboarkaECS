#include "TestScene.h"

#include <format>

#include "components/BaseComponent.h"
#include "components/GameStateComponent.h"
#include "components/HealthComponent.h"
#include "components/TargetComponent.h"
#include "components/WaveStateComponent.h"
#include "ecs/Logger.h"
#include "ecs/components/TagComponent.h"

TestScene::TestScene(float screenW, float screenH) : 
inputSys(screenW, screenH),
movementSys(screenW, screenH), 
waveSpawnerSys(screenW, screenH), 
placementSys(screenW, screenH, _grid)
{
    _screenH = screenH;
    _screenW = screenW;
}

void TestScene::load(World& world)
{
    //Texture2D sprite = LoadTexture("res/idle_01.png");
    SIBOLOG_DEBUG(std::format("Loaded"));
    
    // Entity enemy = world.createEntity();
    // world.addComponent(enemy, TransformComponent{100.f, 100.f, 0.f, 0.f});
    // world.addComponent(enemy, EnemyComponent{70.f, 200});
    // world.addComponent(enemy, HealthComponent{100, 100});
    // world.addComponent(enemy, RenderComponent{15, 15, RED});
    // world.addComponent(enemy, ColliderComponent{15, 15, false});
    
    Entity state = world.createEntity();
    world.addComponent(state, GameStateComponent{});
    world.addComponent(state, WaveStateComponent{2, 10});
    
    Entity playerBase = world.createEntity();
    world.addComponent(playerBase, BaseComponent{});
    world.addComponent(playerBase, TransformComponent{_screenW/2 - 25, _screenH/2 -25, 0, 0});
    world.addComponent(playerBase, HealthComponent{1000, 1000});
    world.addComponent(playerBase, ColliderComponent{50,50});
    world.addComponent(playerBase, RenderComponent{50, 50, BLUE});
    world.addComponent(playerBase, TargetComponent{{_screenW/2 - 25, _screenH/2 - 25}});
    world.addComponent(playerBase, TagComponent{"Base"});
    
}

void TestScene::update(World& world, float deltaTime)
{
    
    if (!gameStateSys.bGameOver)
    {
        inputSys.update(world, deltaTime);
        waveSpawnerSys.update(world, deltaTime);
        pathSys.update(world,deltaTime);
        shootingSys.update(world, deltaTime);
        movementSys.update(world, deltaTime);
        collisionSys.update(world, deltaTime);
        damageSys.update(world, deltaTime);
        gameStateSys.update(world, deltaTime);
        deathSys.update(world, deltaTime);
        placementSys.update(world, deltaTime);
        vfxSys.update(world, deltaTime);
        //physicsResponseSys.update(world,deltaTime);
    }

    renderSys.update(world, deltaTime);
    uiSys.update(world, deltaTime);
    
    if (gameStateSys.bGameOver) DrawGameOver();

    
    world.events.clear();
}

void TestScene::unload(World& world)
{
}

void TestScene::setInputState(const InputState& state)
{
    inputSys.setInputState(state);
}

void TestScene::DrawGameOver()
{
    DrawText("GAME OVER", _screenW/2, _screenH/2, 25, RED);
}
