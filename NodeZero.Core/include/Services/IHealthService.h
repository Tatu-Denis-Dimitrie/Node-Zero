#pragma once

class IHealthService {
   public:
    virtual ~IHealthService() = default;

    virtual void Initialize(float maxHealth, float regenRate) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Reduce(float amount) = 0;
    virtual void RestoreToMax() = 0;

    virtual float GetCurrent() const = 0;
    virtual float GetMax() const = 0;
    virtual bool IsZero() const = 0;
};
