#include "../include/INode.h"

class Position;

#include "Position.cpp"

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
    Node(NodeShape shape, float size, float speed)
        : m_Position(0.0f, 0.0f)
        , m_Shape(shape)
        , m_State(NodeState::Inactive)
        , m_Size(size)
        , m_Speed(speed)
    {
        m_MaxHP = size * 2.0f;
        m_HP = m_MaxHP;
    }

    IPosition& GetPosition() override
    {
        return m_Position;
    }

    const IPosition& GetPosition() const override
    {
        return m_Position;
    }

    NodeShape GetShape() const override
    {
        return m_Shape;
    }

    NodeState GetState() const override
    {
        return m_State;
    }

    float GetSize() const override
    {
        return m_Size;
    }

    float GetSpeed() const override
    {
        return m_Speed;
    }

    float GetHP() const override
    {
        return m_HP;
    }

    float GetMaxHP() const override
    {
        return m_MaxHP;
    }

    void TakeDamage(float damage) override
    {
        if (m_State != NodeState::Active)
            return;

        m_HP -= damage;
        if (m_HP <= 0.0f)
        {
            m_HP = 0.0f;
            Kill();
        }
    }

    void Spawn(float x, float y) override
    {
        m_Position.SetX(x);
        m_Position.SetY(y);
        m_State = NodeState::Active;
        m_HP = m_MaxHP;
    }

    void Kill() override
    {
        m_State = NodeState::Dead;
    }

    void Update(float deltaTime) override
    {
        if (m_State != NodeState::Active)
            return;

        m_Position.Move(-m_Speed * deltaTime, 0.0f);
    }
};
