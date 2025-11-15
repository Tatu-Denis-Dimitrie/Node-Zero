#include "Node.h"
#include "Position.h"

Node::Node(NodeShape shape, float size, float speed)
    : m_Position(0.0f, 0.0f)
    , m_Shape(shape)
    , m_State(NodeState::Inactive)
    , m_Size(size)
    , m_Speed(speed)
{
    m_MaxHP = size * 2.0f;
    m_HP = m_MaxHP;
}

IPosition& Node::GetPosition()
{
    return m_Position;
}

const IPosition& Node::GetPosition() const
{
    return m_Position;
}

NodeShape Node::GetShape() const
{
    return m_Shape;
}

NodeState Node::GetState() const
{
    return m_State;
}

float Node::GetSize() const
{
    return m_Size;
}

float Node::GetSpeed() const
{
    return m_Speed;
}

float Node::GetHP() const
{
    return m_HP;
}

float Node::GetMaxHP() const
{
    return m_MaxHP;
}

void Node::TakeDamage(float damage)
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

void Node::Spawn(float x, float y)
{
    m_Position.SetX(x);
    m_Position.SetY(y);
    m_State = NodeState::Active;
    m_HP = m_MaxHP;
}

void Node::Kill()
{
    m_State = NodeState::Dead;
}

void Node::Update(float deltaTime)
{
    if (m_State != NodeState::Active)
        return;

    m_Position.Move(-m_Speed * deltaTime, 0.0f);
}
