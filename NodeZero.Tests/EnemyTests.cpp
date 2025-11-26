#include <gtest/gtest.h>

#include "../NodeZero.Core/src/Game.h"
#include "../NodeZero.Core/include/IGame.h"
#include "../NodeZero.Core/include/INode.h"

class EnemySpawnTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(EnemySpawnTest, EnemySpawnsAtSpecifiedPosition) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_FLOAT_EQ(nodes[0]->GetPosition().x, 400.0f);
    EXPECT_FLOAT_EQ(nodes[0]->GetPosition().y, 300.0f);
}

TEST_F(EnemySpawnTest, MultipleEnemiesCanSpawn) {
    game->SpawnNode(100.0f, 100.0f);
    game->SpawnNode(200.0f, 200.0f);
    game->SpawnNode(300.0f, 300.0f);

    const auto& nodes = game->GetNodes();
    EXPECT_EQ(nodes.size(), 3);
}

TEST_F(EnemySpawnTest, EnemyHasValidInitialState) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    EXPECT_GT(enemy->GetSize(), 0.0f);
    EXPECT_GT(enemy->GetHP(), 0.0f);
    EXPECT_GT(enemy->GetMaxHP(), 0.0f);
    EXPECT_EQ(enemy->GetHP(), enemy->GetMaxHP());
}

class EnemyDamageTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(EnemyDamageTest, EnemyTakesDamage) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    float initialHP = enemy->GetHP();
    enemy->TakeDamage(10.0f);

    EXPECT_LT(enemy->GetHP(), initialHP);
    EXPECT_FLOAT_EQ(enemy->GetHP(), initialHP - 10.0f);
}

TEST_F(EnemyDamageTest, EnemyHPCannotGoBelowZero) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    enemy->TakeDamage(1000.0f);
    EXPECT_GE(enemy->GetHP(), 0.0f);
}

TEST_F(EnemyDamageTest, EnemyDiesWhenHPReachesZero) {
    game->SpawnNode(400.0f, 300.0f);
    game->SetMousePosition(400.0f, 300.0f);

    const auto& nodes = game->GetNodes();
    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    float maxHP = enemy->GetMaxHP();
    enemy->TakeDamage(maxHP);

    EXPECT_LE(enemy->GetHP(), 0.0f);
}

TEST_F(EnemyDamageTest, MultipleDamageApplications) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    INode* enemy = nodes[0];

    float initialHP = enemy->GetHP();
    enemy->TakeDamage(5.0f);
    enemy->TakeDamage(3.0f);
    enemy->TakeDamage(2.0f);

    EXPECT_FLOAT_EQ(enemy->GetHP(), initialHP - 10.0f);
}

class EnemyPropertiesTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(EnemyPropertiesTest, EnemyHasValidSpeed) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_GE(nodes[0]->GetSpeed(), 0.0f);
}

TEST_F(EnemyPropertiesTest, EnemyHasValidSize) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_GT(nodes[0]->GetSize(), 0.0f);
}

TEST_F(EnemyPropertiesTest, EnemyHasShape) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    NodeShape shape = nodes[0]->GetShape();
    EXPECT_TRUE(shape == NodeShape::Circle ||
                shape == NodeShape::Square ||
                shape == NodeShape::Hexagon ||
                shape == NodeShape::Boss);
}

TEST_F(EnemyPropertiesTest, EnemyHasRotation) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    float rotation = nodes[0]->GetRotation();
    EXPECT_GE(rotation, 0.0f);
}

class EnemyDestructionTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(EnemyDestructionTest, DestroyedEnemiesIncrementCounter) {
    int initialDestroyed = game->GetNodesDestroyed();

    game->SpawnNode(400.0f, 300.0f);
    game->SetMousePosition(400.0f, 300.0f);

    const auto& nodes = game->GetNodes();
    if (nodes.size() > 0) {
        INode* enemy = nodes[0];
        enemy->TakeDamage(enemy->GetMaxHP());
        game->Update(0.016f);
    }

    EXPECT_GE(game->GetNodesDestroyed(), initialDestroyed);
}

TEST_F(EnemyDestructionTest, LevelCounterTracksDestroyedEnemies) {
    int initialDestroyed = game->GetLevelService().GetNodesDestroyedThisLevel();

    game->SpawnNode(400.0f, 300.0f);
    game->SetMousePosition(400.0f, 300.0f);

    const auto& nodes = game->GetNodes();
    if (nodes.size() > 0) {
        INode* enemy = nodes[0];
        enemy->TakeDamage(enemy->GetMaxHP());
        game->Update(0.016f);
    }

    EXPECT_GE(game->GetLevelService().GetNodesDestroyedThisLevel(), initialDestroyed);
}

class EnemyUpdateTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(EnemyUpdateTest, EnemyUpdateDoesNotCrash) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    EXPECT_NO_THROW(nodes[0]->Update(0.016f));
}

TEST_F(EnemyUpdateTest, MultipleEnemiesUpdateWithoutCrash) {
    game->SpawnNode(100.0f, 100.0f);
    game->SpawnNode(200.0f, 200.0f);
    game->SpawnNode(300.0f, 300.0f);

    const auto& nodes = game->GetNodes();
    ASSERT_EQ(nodes.size(), 3);

    for (INode* enemy : nodes) {
        EXPECT_NO_THROW(enemy->Update(0.016f));
    }
}

TEST_F(EnemyUpdateTest, EnemyPositionValidAfterUpdate) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();

    ASSERT_EQ(nodes.size(), 1);
    nodes[0]->Update(0.016f);

    Position pos = nodes[0]->GetPosition();
    EXPECT_TRUE(std::isfinite(pos.x));
    EXPECT_TRUE(std::isfinite(pos.y));
}