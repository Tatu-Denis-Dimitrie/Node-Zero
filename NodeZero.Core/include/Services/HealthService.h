#pragma once

#include "IHealthService.h"

class HealthService : public IHealthService {
   private:
    float m_MaxHealth;
    float m_CurrentHealth;
    float m_RegenRate;
    float m_HealthDepletionRate;
    float m_HealthDepletionInterval;
    float m_HealthTimer;
    int m_CurrentLevel;

   public:
    HealthService();
    ~HealthService() override = default;

    void Initialize(float maxHealth, float regenRate) override;
    void Update(float deltaTime) override;
    void Reset(float maxHealth);

    void Reduce(float amount) override;
    void SetMaxHealth(float maxHealth);
    void SetRegenRate(float regenRate);
    void SetCurrentLevel(int level);
    void RestoreToMax() override;

    float GetCurrent() const override;
    float GetMax() const override;
    bool IsZero() const override;

   private:
    void ApplyRegeneration(float deltaTime);
    void ApplyDepletion(float deltaTime);
};
