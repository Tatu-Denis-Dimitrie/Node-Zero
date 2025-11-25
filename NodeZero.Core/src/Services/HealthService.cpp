#include "HealthService.h"

#include "Config/GameConfig.h"

HealthService::HealthService()
    : m_MaxHealth(GameConfig::HEALTH_DEFAULT),
      m_CurrentHealth(GameConfig::HEALTH_DEFAULT),
      m_RegenRate(0.0f),
      m_HealthDepletionRate(0.1f),
      m_HealthDepletionInterval(0.3f),
      m_HealthTimer(0.0f),
      m_CurrentLevel(1) {
}

void HealthService::Initialize(float maxHealth, float regenRate) {
    m_MaxHealth = maxHealth;
    m_CurrentHealth = maxHealth;
    m_RegenRate = regenRate;
}

void HealthService::Update(float deltaTime) {
    ApplyRegeneration(deltaTime);
    ApplyDepletion(deltaTime);
}

void HealthService::Reset(float maxHealth) {
    m_MaxHealth = maxHealth;
    m_CurrentHealth = maxHealth;
    m_HealthTimer = 0.0f;
}

void HealthService::Reduce(float amount) {
    m_CurrentHealth -= amount;
    if (m_CurrentHealth < 0.0f) {
        m_CurrentHealth = 0.0f;
    }
}

void HealthService::SetMaxHealth(float maxHealth) {
    m_MaxHealth = maxHealth;
}

void HealthService::SetRegenRate(float regenRate) {
    m_RegenRate = regenRate;
}

void HealthService::SetCurrentLevel(int level) {
    m_CurrentLevel = level;
}

void HealthService::RestoreToMax() {
    m_CurrentHealth = m_MaxHealth;
}

float HealthService::GetCurrent() const {
    return m_CurrentHealth;
}

float HealthService::GetMax() const {
    return m_MaxHealth;
}

bool HealthService::IsZero() const {
    return m_CurrentHealth <= 0.0f;
}

void HealthService::ApplyRegeneration(float deltaTime) {
    if (m_RegenRate > 0.0f && m_CurrentHealth < m_MaxHealth && m_CurrentHealth > 0.0f) {
        m_CurrentHealth += m_RegenRate * deltaTime;
        if (m_CurrentHealth > m_MaxHealth) {
            m_CurrentHealth = m_MaxHealth;
        }
    }
}

void HealthService::ApplyDepletion(float deltaTime) {
    m_HealthTimer += deltaTime;
    if (m_HealthTimer >= m_HealthDepletionInterval) {
        float scaledDepletionRate = m_HealthDepletionRate * (1.0f + (m_CurrentLevel - 1) * 0.20f);
        m_CurrentHealth -= scaledDepletionRate;
        m_HealthTimer = 0.0f;

        if (m_CurrentHealth < 0.0f) {
            m_CurrentHealth = 0.0f;
        }
    }
}
