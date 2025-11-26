#pragma once

#include "../Enums/NodeShape.h"

class ISpawnService {
   public:
    virtual ~ISpawnService() = default;

    virtual void UpdateAutoSpawn(float deltaTime) = 0;

    virtual NodeShape GetRandomShape() const = 0;
    virtual float CalculateNodeHP(float baseHP) const = 0;

    virtual bool ShouldAutoSpawn() const = 0;
};
