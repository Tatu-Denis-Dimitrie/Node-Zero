#include "Level.h"

Level::Level(int difficulty)
    : m_Difficulty(difficulty)
    , m_IsLoaded(false)
    , m_IsCompleted(false)
    , m_ElapsedTime(0.0f)
{
}

void Level::Load()
{
    m_IsLoaded = true;
    m_IsCompleted = false;
    m_ElapsedTime = 0.0f;
}

void Level::Unload()
{
    m_IsLoaded = false;
}

void Level::Update(float deltaTime)
{
    if (!m_IsLoaded || m_IsCompleted)
        return;

    m_ElapsedTime += deltaTime;
}

bool Level::IsCompleted() const
{
    return m_IsCompleted;
}

int Level::GetDifficulty() const
{
    return m_Difficulty;
}
