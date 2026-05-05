#pragma once
#include "raylib.h"
#include "ecs/Types.h"

struct cell
{
    int x;
    int y;
};

struct PlaceTowerRequestEvent
{
    cell gridCell;
    Vector2 coords;
    Entity entity;
};
