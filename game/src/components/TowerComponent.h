#pragma once

struct TowerComponent
{
    float fireRate = 1.f;
    float damage = 50.f;
    float range = 100000.f;
    float cooldownTimer = 1.f / fireRate;
};