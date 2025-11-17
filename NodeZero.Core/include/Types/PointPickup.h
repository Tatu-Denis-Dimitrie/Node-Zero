#pragma once

#include "Position.h"

struct PointPickup
{
    int id;
    Position position;
    Position spawnOrigin;
    float size;
    float lifetime;
    float remainingTime;
    int points;

    float GetLifeRatio() const
    {
        if (lifetime <= 0.0f)
        {
            return 0.0f;
        }
        return remainingTime / lifetime;
    }

    float GetAge() const
    {
        return lifetime - remainingTime;
    }
};

