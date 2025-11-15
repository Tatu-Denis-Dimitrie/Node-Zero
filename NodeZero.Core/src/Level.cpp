#include "../include/ILevel.h"

/// <summary>
/// Implementare concretă a unui nivel de joc
/// </summary>
class Level : public ILevel
{
private:
    int m_Difficulty;
    bool m_IsLoaded;
    bool m_IsCompleted;
    float m_ElapsedTime;

public:
    Level(int difficulty = 1)
        : m_Difficulty(difficulty)
        , m_IsLoaded(false)
        , m_IsCompleted(false)
        , m_ElapsedTime(0.0f)
    {
    }

    void Load() override
    {
        // Încarcă resursele nivelului
        m_IsLoaded = true;
        m_IsCompleted = false;
        m_ElapsedTime = 0.0f;
    }

    void Unload() override
    {
        // Descarcă resursele nivelului
        m_IsLoaded = false;
    }

    void Update(float deltaTime) override
    {
        if (!m_IsLoaded || m_IsCompleted)
            return;

        m_ElapsedTime += deltaTime;

        // Logica de nivel (spawn noduri, verifică condiții de completare, etc.)
        // TODO: Implementare completă
    }

    bool IsCompleted() const override
    {
        return m_IsCompleted;
    }

    int GetDifficulty() const override
    {
        return m_Difficulty;
    }
};
