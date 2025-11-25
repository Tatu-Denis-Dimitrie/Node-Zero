#pragma once

class IPointsSystem {
   public:
    virtual ~IPointsSystem() = default;

    virtual void AddPoints(int points) = 0;
    virtual int GetPoints() const = 0;
    virtual void ResetPoints() = 0;
    virtual void IncreaseMultiplier() = 0;
    virtual void ResetMultiplier() = 0;
    virtual int GetMultiplier() const = 0;
    virtual int GetHighPoints() const = 0;
    virtual void UpdateHighPoints() = 0;
};
