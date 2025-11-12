#include "Game.h"

Game::Game()
    : m_Player(), m_ScreenWidth(0.0f), m_ScreenHeight(0.0f)
{
}

void Game::Initialize(float screenWidth, float screenHeight)
{
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
    
    m_Player.SetRotation(Rotation::North);
}

void Game::Update(float deltaTime)
{
    m_Player.Update(deltaTime);
}

IPlayer& Game::GetPlayer()
{
    return m_Player;
}

const IPlayer& Game::GetPlayer() const
{
    return m_Player;
}
