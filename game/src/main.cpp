#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include <ecs/World.h>

#include "components/HealthComponent.h"
#include "ecs/components/TagComponent.h"
#include "scenes/TestScene.h"

int main() {

    const int screenW = 1920;
    const int screenH = 1080;

    InitWindow(screenW, screenH, "ECS Engine");

    SetTargetFPS(60);

    World world;
    TestScene scene(900, 900);
    
    RenderTexture2D gameTexture = LoadRenderTexture(900, 900);
    
    ImVec2 panelPos = {0, 0};
    
    Entity selectedEntity = NULL_ENTITY;
    bool bShouldPlay = false;
    scene.load(world);
    rlImGuiSetup(true);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        float dt = GetFrameTime();
        
        Vector2 mouse = GetMousePosition();
        world.input.mouseX = mouse.x - panelPos.x;
        world.input.mouseY = mouse.y - panelPos.y;
        
        rlImGuiBegin();
        
        //Render game to texture
        BeginTextureMode(gameTexture);
        
        for (int i = 0; i < 31; ++i)
        {
            DrawLine(30*i, 0, 30*i, 900, Color{255,255,255, 50});
        }
        for (int i = 0; i < 31; ++i)
        {
            DrawLine(0, 30*i, 900, 30*i, Color{255,255,255, 50});
        }
        ClearBackground(BLACK);
        
        
        world.input.imguiCapturingMouse = ImGui::GetIO().WantCaptureMouse;

        if (bShouldPlay)
        {
            scene.update(world, dt);
        }else
        {
            scene.update(world, 0);
        }
        EndTextureMode();
        
        
        // Play Pause

        ImGui::Begin("Play/Pause");
        ImGui::Checkbox("Play", &bShouldPlay);
        ImGui::End();
        
        // Viewport
        ImGui::Begin("Viewport");
        panelPos = ImGui::GetCursorScreenPos();
        world.input.bMouseInViewport = mouse.x >= panelPos.x && mouse.x < panelPos.x + 900 && mouse.y >= panelPos.y && mouse.y < panelPos.y + 900;
        rlImGuiImageRenderTexture(&gameTexture);
        ImGui::End();
        
        // Outliner
        ImGui::Begin("Outliner");
        for (Entity e : world.getEntitiesWith<TagComponent>())
        {
            std::string label = world.getComponent<TagComponent>(e).name + "##" + std::to_string(e);

            if (ImGui::Selectable(label.c_str(), selectedEntity == e))
            {
                selectedEntity = e;
            }
        }
        ImGui::End();
        
        // Inspector
        ImGui::Begin("Inspector");
        if (selectedEntity != NULL_ENTITY)
        {
            if (world.hasComponent<TransformComponent>(selectedEntity))
            {
                auto& t  = world.getComponent<TransformComponent>(selectedEntity);
                ImGui::Text("Transform");
                ImGui::DragFloat("X", &t.x);
                ImGui::DragFloat("Y", &t.y);
            }
            
            if (world.hasComponent<HealthComponent>(selectedEntity)) {
                auto& h = world.getComponent<HealthComponent>(selectedEntity);
                ImGui::Text("Health");
                ImGui::DragFloat("HP", &h.currentHP);
            }
        }
        ImGui::End();
        
        rlImGuiEnd();
        
        DrawFPS(10,10);
        EndDrawing();
    }
    scene.unload(world);
    UnloadRenderTexture(gameTexture);
    CloseWindow();
    return 0;
}
