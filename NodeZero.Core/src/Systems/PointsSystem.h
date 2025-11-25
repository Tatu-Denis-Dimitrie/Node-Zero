#pragma once

#include "../../include/Systems/IPointsSystem.h"

class PointsSystem : public IPointsSystem {
   private:
    int m_Points;
    int m_HighPoints;
    int m_Multiplier;

   public:
    PointsSystem();

    void AddPoints(int points) override;
    int GetPoints() const override;
    void ResetPoints() override;

    void IncreaseMultiplier() override;
    void ResetMultiplier() override;
    int GetMultiplier() const override;

    int GetHighPoints() const override;
    void UpdateHighPoints() override;
};
