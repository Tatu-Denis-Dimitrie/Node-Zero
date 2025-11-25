#pragma once

class HealthService {
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
    ~HealthService() = default;

    void Initialize(float maxHealth, float regenRate);
    void Update(float deltaTime);
    void Reset(float maxHealth);

    void Reduce(float amount);
    void SetMaxHealth(float maxHealth);
    void SetRegenRate(float regenRate);
    void SetCurrentLevel(int level);
    void RestoreToMax();

    float GetCurrent() const;
    float GetMax() const;
    bool IsZero() const;

   private:
    void ApplyRegeneration(float deltaTime);
    void ApplyDepletion(float deltaTime);
};
