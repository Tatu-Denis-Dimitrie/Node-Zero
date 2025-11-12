#pragma once

#include "../Core/IGame.h"
#include "Player.h"

class Game : public IGame
{
private:
    Player m_Player;
    float m_ScreenWidth;
    float m_ScreenHeight;

public:
    Game();

    void Initialize(float screenWidth, float screenHeight) override;
    void Update(float deltaTime) override;
    
    IPlayer& GetPlayer() override;
    const IPlayer& GetPlayer() const override;
    
    float GetScreenWidth() const { return m_ScreenWidth; }
    float GetScreenHeight() const { return m_ScreenHeight; }
};
