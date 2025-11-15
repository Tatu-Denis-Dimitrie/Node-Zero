#include "../../include/Systems/IScoreSystem.h"
#include "../../include/Config/GameConfig.h"
#include <algorithm>

class ScoreSystem : public IScoreSystem
{
private:
    int m_Score;
    int m_HighScore;
    int m_Multiplier;

public:
    ScoreSystem()
        : m_Score(0)
        , m_HighScore(0)
        , m_Multiplier(1)
    {
    }

    void AddScore(int points) override
    {
        m_Score += points * m_Multiplier;
    }

    int GetScore() const override
    {
        return m_Score;
    }

    void ResetScore() override
    {
        m_Score = 0;
    }

    void IncreaseMultiplier() override
    {
        m_Multiplier = std::min(m_Multiplier + 1, GameConfig::POINTS_MULTIPLIER_MAX);
    }

    void ResetMultiplier() override
    {
        m_Multiplier = 1;
    }

    int GetMultiplier() const override
    {
        return m_Multiplier;
    }

    int GetHighScore() const override
    {
        return m_HighScore;
    }

    void UpdateHighScore() override
    {
        if (m_Score > m_HighScore)
        {
            m_HighScore = m_Score;
        }
    }
};
