#include "ScoreSystem.h"
#include "../../include/Config/GameConfig.h"
#include <algorithm>

ScoreSystem::ScoreSystem()
    : m_Score(0)
    , m_HighScore(0)
    , m_Multiplier(1)
{
}

void ScoreSystem::AddScore(int points)
{
    m_Score += points * m_Multiplier;
}

int ScoreSystem::GetScore() const
{
    return m_Score;
}

void ScoreSystem::ResetScore()
{
    m_Score = 0;
}

void ScoreSystem::IncreaseMultiplier()
{
    m_Multiplier = std::min(m_Multiplier + 1, GameConfig::POINTS_MULTIPLIER_MAX);
}

void ScoreSystem::ResetMultiplier()
{
    m_Multiplier = 1;
}

int ScoreSystem::GetMultiplier() const
{
    return m_Multiplier;
}

int ScoreSystem::GetHighScore() const
{
    return m_HighScore;
}

void ScoreSystem::UpdateHighScore()
{
    if (m_Score > m_HighScore)
    {
        m_HighScore = m_Score;
    }
}
