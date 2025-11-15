#pragma once

#include "../include/INode.h"
#include "Position.h"

class Node : public INode
{
private:
    Position m_Position;
    NodeShape m_Shape;
    NodeState m_State;
    float m_Size;
    float m_Speed;
    float m_HP;
    float m_MaxHP;

public:
    Node(NodeShape shape, float size, float speed);

    IPosition& GetPosition() override;
    const IPosition& GetPosition() const override;

    NodeShape GetShape() const override;
    NodeState GetState() const override;

    float GetSize() const override;
    float GetSpeed() const override;

    float GetHP() const override;
    float GetMaxHP() const override;

    void Spawn(float x, float y) override;
    void Update(float deltaTime) override;
    void Kill() override;
    void TakeDamage(float damage) override;
};
