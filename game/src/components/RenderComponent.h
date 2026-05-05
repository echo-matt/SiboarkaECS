#pragma once
#include <raylib.h>

enum EType { 
    Box,
    Sphere,
    Sprite
};

struct RenderComponent
{
    float width  = 20.f;
    float height = 20.f;
    Color color  = RED;
    float radius = 20.f;
    EType type = Box;
};
