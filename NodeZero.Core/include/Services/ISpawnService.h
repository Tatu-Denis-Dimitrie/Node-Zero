#pragma once

#include "../Types/SpawnInfo.h"

class ISpawnService {
   public:
    virtual ~ISpawnService() = default;

    virtual void UpdateAutoSpawn(float deltaTime) = 0;
    virtual bool ShouldAutoSpawn() const = 0;

    virtual SpawnInfo GetNextSpawn() const = 0;
    virtual float CalculateNodeHP(float baseHP) const = 0;
};
