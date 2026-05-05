#pragma once
#include <raylib.h>


struct SpriteComponent
{
    Texture2D texture = {};
    Rectangle sourceRect = {0,0,0,0};
    Vector2 origin = {0.f, 0.f};
    float rotation = 0.f;
    Color tint = WHITE;
};