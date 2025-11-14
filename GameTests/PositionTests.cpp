#include <gtest/gtest.h>
#include "Entities/Position.h"

class PositionTest : public ::testing::Test
{
protected:
    Position* position;

    void SetUp() override
    {
        position = new Position(0.0f, 0.0f);
    }

    void TearDown() override
    {
        delete position;
    }
};

TEST_F(PositionTest, DefaultConstructorSetsToZero)
{
    Position pos;
    EXPECT_FLOAT_EQ(pos.GetX(), 0.0f);
    EXPECT_FLOAT_EQ(pos.GetY(), 0.0f);
}

TEST_F(PositionTest, ConstructorWithParametersSetsCorrectly)
{
    Position pos(100.0f, 200.0f);
    EXPECT_FLOAT_EQ(pos.GetX(), 100.0f);
    EXPECT_FLOAT_EQ(pos.GetY(), 200.0f);
}

TEST_F(PositionTest, SetXWorks)
{
    position->SetX(50.0f);
    EXPECT_FLOAT_EQ(position->GetX(), 50.0f);
}

TEST_F(PositionTest, SetYWorks)
{
    position->SetY(75.0f);
    EXPECT_FLOAT_EQ(position->GetY(), 75.0f);
}

TEST_F(PositionTest, MoveUpdatesPosition)
{
    position->SetX(100.0f);
    position->SetY(100.0f);

    position->Move(10.0f, 20.0f);

    EXPECT_FLOAT_EQ(position->GetX(), 110.0f);
    EXPECT_FLOAT_EQ(position->GetY(), 120.0f);
}

TEST_F(PositionTest, MoveWithNegativeValues)
{
    position->SetX(100.0f);
    position->SetY(100.0f);

    position->Move(-50.0f, -25.0f);

    EXPECT_FLOAT_EQ(position->GetX(), 50.0f);
    EXPECT_FLOAT_EQ(position->GetY(), 75.0f);
}
