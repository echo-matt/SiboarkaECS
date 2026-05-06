#pragma once
#include <ecs/System.h>
#include <ecs/World.h>
#include <ecs/components/TransformComponent.h>
#include <components/RenderComponent.h>
#include <raylib.h>

#include "components/LifetimeComponent.h"
#include "components/SpriteComponent.h"
#include "components/VFX/LaserComponent.h"

class RenderSystem : public System
{
public:
    void update(World& world, float /*deltaTime*/) override
    {
        for (Entity e : world.getEntitiesWith<TransformComponent, RenderComponent>())
        {
            const auto& t = world.getComponent<TransformComponent>(e);
            const auto& r = world.getComponent<RenderComponent>(e);

            switch (r.type)
            {
            case Box:
                DrawRectangle(
                    static_cast<int>(t.x),
                    static_cast<int>(t.y),
                    static_cast<int>(r.width),
                    static_cast<int>(r.height),
                    r.color
                );
                break;
            case Sphere:
                DrawCircle(t.x, t.y, r.radius, r.color);
                break;
            }
        }
        for (Entity e : world.getEntitiesWith<TransformComponent, SpriteComponent>())
        {
            const auto& t = world.getComponent<TransformComponent>(e);
            const auto& s = world.getComponent<SpriteComponent>(e);
            
            Rectangle destRect = {
                t.x,
                t.y,
                16,
                16
            };
            
            Rectangle src = { 0.f, 0.f,
                static_cast<float>(s.texture.width),
                static_cast<float>(s.texture.height) };
            
            DrawTexturePro(s.texture, src, destRect, s.origin, s.rotation, s.tint);
        }
        
        for (Entity e : world.getEntitiesWith<LaserComponent, LifetimeComponent>())
        {
            auto laserComp = world.getComponent<LaserComponent>(e);
            DrawLineEx({laserComp.x1, laserComp.y1}, {laserComp.x2, laserComp.y2}, 1.f, laserComp.color);
        }
    }
};
