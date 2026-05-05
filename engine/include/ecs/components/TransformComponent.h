#pragma once

struct TransformComponent {
    float x    = 0.f;
    float y    = 0.f;
    mutable float velX = 0.f;  
    mutable float velY = 0.f;  
};
