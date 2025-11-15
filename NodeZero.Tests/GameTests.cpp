#include <gtest/gtest.h>
#include "Core/GameFactory.h"
#include "Core/IGame.h"

class GameTest : public ::testing::Test
{
protected:
    IGame* game;

    void SetUp() override
    {
        game = GameFactory::CreateGame();
    }

    void TearDown() override
    {
        GameFactory::DestroyGame(game);
    }
};

TEST_F(GameTest, GameFactoryCreatesGame)
{
    EXPECT_NE(game, nullptr);
}

TEST_F(GameTest, InitializeDoesNotCrash)
{
    EXPECT_NO_THROW(game->Initialize(800.0f, 600.0f));
}

TEST_F(GameTest, GetPlayerReturnsValidPlayer)
{
    game->Initialize(800.0f, 600.0f);
    EXPECT_NO_THROW(game->GetPlayer());
}

TEST_F(GameTest, InitialEnemyListIsEmpty)
{
    game->Initialize(800.0f, 600.0f);
    EXPECT_EQ(game->GetEnemies().size(), 0);
}

TEST_F(GameTest, SpawnEnemyAddsEnemy)
{
    game->Initialize(800.0f, 600.0f);
    game->SpawnEnemy(EnemyShape::Circle, 100.0f, 100.0f, 20.0f, 100.0f);

    EXPECT_EQ(game->GetEnemies().size(), 1);
}

TEST_F(GameTest, SpawnMultipleEnemies)
{
    game->Initialize(800.0f, 600.0f);

    game->SpawnEnemy(EnemyShape::Circle, 100.0f, 100.0f, 20.0f, 100.0f);
    game->SpawnEnemy(EnemyShape::Triangle, 200.0f, 100.0f, 25.0f, 150.0f);
    game->SpawnEnemy(EnemyShape::Square, 300.0f, 100.0f, 30.0f, 200.0f);

    EXPECT_EQ(game->GetEnemies().size(), 3);
}

TEST_F(GameTest, UpdateDoesNotCrash)
{
    game->Initialize(800.0f, 600.0f);
    EXPECT_NO_THROW(game->Update(0.016f));
}

TEST_F(GameTest, UpdateWithEnemiesDoesNotCrash)
{
    game->Initialize(800.0f, 600.0f);
    game->SpawnEnemy(EnemyShape::Circle, 100.0f, 100.0f, 20.0f, 100.0f);

    EXPECT_NO_THROW(game->Update(0.016f));
}
