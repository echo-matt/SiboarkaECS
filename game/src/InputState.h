#pragma once

struct InputState
{
public:
    float mouseX, mouseY = 0;
    bool bImguiCapturingMouse = false;
    bool bMouseInViewport = false;
};