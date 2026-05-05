#include <raylib.h>

#include <ecs/World.h>

#include "scenes/TestScene.h"

int main() {

    const int screenW = 900;
    const int screenH = 900;

    InitWindow(screenW, screenH, "ECS Engine");

    SetTargetFPS(60);

    World world;
    TestScene scene(screenW, screenH);
    
    scene.load(world);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        scene.update(world, dt);
    }
    scene.unload(world);
    CloseWindow();
    return 0;
}
