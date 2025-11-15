#pragma once

#include "../../include/Systems/IScoreSystem.h"

/// <summary>
/// Implementare sistem de punctaj cu multiplier și high score
/// </summary>
class ScoreSystem : public IScoreSystem
{
private:
    int m_Score;
    int m_HighScore;
    int m_Multiplier;

public:
    ScoreSystem();

    void AddScore(int points) override;
    int GetScore() const override;
    void ResetScore() override;

    void IncreaseMultiplier() override;
    void ResetMultiplier() override;
    int GetMultiplier() const override;

    int GetHighScore() const override;
    void UpdateHighScore() override;
};
