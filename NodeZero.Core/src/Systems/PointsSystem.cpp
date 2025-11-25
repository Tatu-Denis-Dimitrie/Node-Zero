#include "PointsSystem.h"
#include "../../include/Config/GameConfig.h"
#include <algorithm>

PointsSystem::PointsSystem()
    : m_Points(0)
    , m_HighPoints(0)
    , m_Multiplier(1)
{
}

void PointsSystem::AddPoints(int points)
{
    m_Points += points * m_Multiplier;
}

int PointsSystem::GetPoints() const
{
    return m_Points;
}

void PointsSystem::ResetPoints()
{
    m_Points = 0;
}

void PointsSystem::IncreaseMultiplier()
{
    m_Multiplier = std::min(m_Multiplier + 1, GameConfig::POINTS_MULTIPLIER_MAX);
}

void PointsSystem::ResetMultiplier()
{
    m_Multiplier = 1;
}

int PointsSystem::GetMultiplier() const
{
    return m_Multiplier;
}

int PointsSystem::GetHighPoints() const
{
    return m_HighPoints;
}

void PointsSystem::UpdateHighPoints()
{
    if (m_Points > m_HighPoints)
    {
        m_HighPoints = m_Points;
    }
}
