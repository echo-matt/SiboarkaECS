#pragma once
#include "raylib.h"

struct cell1
{
    int x;
    int y;
};

class RemoveTowerRequestEvent
{
public:
    cell1 gridCell;
    Vector2 coords;
};
