#pragma once
#include "raylib.h"
#include "ecs/Types.h"

struct BulletComponent
{
    float Radius = 10.f;
    Color color = RED;
    float Damage = 50.f;
    Entity ownerEntity;
};
