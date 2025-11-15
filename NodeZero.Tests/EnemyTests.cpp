#include <gtest/gtest.h>
#include "Entities/Enemy.h"
#include "Enums/EnemyTypes.h"

class EnemyTest : public ::testing::Test
{
protected:
    Enemy* enemy;

    void SetUp() override
    {
        enemy = new Enemy(EnemyShape::Circle, 20.0f, 100.0f);
    }

    void TearDown() override
    {
        delete enemy;
    }
};

TEST_F(EnemyTest, InitialStateIsInactive)
{
    EXPECT_EQ(enemy->GetState(), EnemyState::Inactive);
}

TEST_F(EnemyTest, SpawnSetsStateToActive)
{
    enemy->Spawn(100.0f, 50.0f);
    EXPECT_EQ(enemy->GetState(), EnemyState::Active);
}

TEST_F(EnemyTest, SpawnSetsCorrectPosition)
{
    enemy->Spawn(100.0f, 50.0f);
    EXPECT_FLOAT_EQ(enemy->GetPosition().GetX(), 100.0f);
    EXPECT_FLOAT_EQ(enemy->GetPosition().GetY(), 50.0f);
}

TEST_F(EnemyTest, KillSetsStateToDead)
{
    enemy->Spawn(100.0f, 50.0f);
    enemy->Kill();
    EXPECT_EQ(enemy->GetState(), EnemyState::Dead);
}

TEST_F(EnemyTest, GetShapeReturnsCorrectShape)
{
    EXPECT_EQ(enemy->GetShape(), EnemyShape::Circle);
}

TEST_F(EnemyTest, GetSizeReturnsCorrectSize)
{
    EXPECT_FLOAT_EQ(enemy->GetSize(), 20.0f);
}

TEST_F(EnemyTest, GetSpeedReturnsCorrectSpeed)
{
    EXPECT_FLOAT_EQ(enemy->GetSpeed(), 100.0f);
}

TEST_F(EnemyTest, UpdateMovesEnemy)
{
    enemy->Spawn(100.0f, 50.0f);
    float initialY = enemy->GetPosition().GetY();

    enemy->Update(1.0f); // 1 second

    float newY = enemy->GetPosition().GetY();
    EXPECT_GT(newY, initialY); // Y should increase (moving down)
}
