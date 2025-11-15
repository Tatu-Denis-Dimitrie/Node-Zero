#pragma once

class IScoreSystem
{
public:
    virtual ~IScoreSystem() = default;

    virtual void AddScore(int points) = 0;
    virtual int GetScore() const = 0;
    virtual void ResetScore() = 0;
    virtual void IncreaseMultiplier() = 0;
    virtual void ResetMultiplier() = 0;
    virtual int GetMultiplier() const = 0;
    virtual int GetHighScore() const = 0;
    virtual void UpdateHighScore() = 0;
};
