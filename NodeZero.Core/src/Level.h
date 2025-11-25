#pragma once

#include "ILevel.h"

class Level : public ILevel {
   private:
    int m_Difficulty;
    bool m_IsLoaded;
    bool m_IsCompleted;
    float m_ElapsedTime;

   public:
    Level(int difficulty = 1);

    void Load() override;
    void Unload() override;
    void Update(float deltaTime) override;

    bool IsCompleted() const override;
    int GetDifficulty() const override;
};
