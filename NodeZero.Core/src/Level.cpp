#include "../include/ILevel.h"

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
        m_IsLoaded = true;
        m_IsCompleted = false;
        m_ElapsedTime = 0.0f;
    }

    void Unload() override
    {
        m_IsLoaded = false;
    }

    void Update(float deltaTime) override
    {
        if (!m_IsLoaded || m_IsCompleted)
            return;

        m_ElapsedTime += deltaTime;
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
