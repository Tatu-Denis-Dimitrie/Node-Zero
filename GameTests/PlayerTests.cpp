#include <gtest/gtest.h>
#include "Entities/Player.h"

class PlayerTest : public ::testing::Test
{
protected:
    Player player;

    void SetUp() override
    {
        // Setup code before each test
    }

    void TearDown() override
    {
        // Cleanup code after each test
    }
};

TEST_F(PlayerTest, InitialRotationIsZero)
{
    EXPECT_FLOAT_EQ(player.GetRotationDegrees(), 0.0f);
}

TEST_F(PlayerTest, SetRotationWorks)
{
    player.SetRotationDegrees(45.0f);
    EXPECT_FLOAT_EQ(player.GetRotationDegrees(), 45.0f);
}

TEST_F(PlayerTest, SetRotationNegativeValue)
{
    player.SetRotationDegrees(-90.0f);
    EXPECT_FLOAT_EQ(player.GetRotationDegrees(), -90.0f);
}

TEST_F(PlayerTest, SetRotation360Degrees)
{
    player.SetRotationDegrees(360.0f);
    EXPECT_FLOAT_EQ(player.GetRotationDegrees(), 360.0f);
}

TEST_F(PlayerTest, UpdateDoesNotCrash)
{
    EXPECT_NO_THROW(player.Update(0.016f));
}
