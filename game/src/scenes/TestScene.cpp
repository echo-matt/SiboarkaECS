#include "TestScene.h"

#include <format>

#include "components/DeadComponent.h"
#include "components/EnemyComponent.h"
#include "components/HealthComponent.h"
#include "components/SpriteComponent.h"
#include "ecs/Logger.h"
#include "ecs/components/StaticComponent.h"
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
}

void TestScene::update(World& world, float deltaTime)
{
    deathSys.update(world,deltaTime);
    inputSys.update(world, deltaTime);
    waveSpawnerSys.update(world, deltaTime);
    shootingSys.update(world, deltaTime);
    placementSys.update(world, deltaTime);
    pathSys.update(world,deltaTime);
    movementSys.update(world, deltaTime);
    collisionSys.update(world, deltaTime);
    damageSys.update(world, deltaTime);
    vfxSys.update(world, deltaTime);
    //physicsResponseSys.update(world,deltaTime);
    
    for (Entity e : world.getEntitiesWith<EnemyComponent>())
    {
        //SIBOLOG_DEBUG(std::format("Enemy health: {}", world.getComponent<HealthComponent>(e).currentHP));
    }
    
    BeginDrawing();

    ClearBackground(BLACK);

    for (int i = 0; i < 31; ++i)
    {
        DrawLine(30*i, 0, 30*i, _screenH, Color{255,255,255, 50});
    }
    for (int i = 0; i < 31; ++i)
    {
        DrawLine(0, 30*i, _screenW, 30*i, Color{255,255,255, 50});
    }

    renderSys.update(world, deltaTime);

    DrawFPS(10, 10);

    EndDrawing();

    world.events.clear();
}

void TestScene::unload(World& world)
{
}
