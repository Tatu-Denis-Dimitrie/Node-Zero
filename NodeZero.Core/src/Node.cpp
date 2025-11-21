#include "Node.h"

Node::Node(NodeShape shape, float size, float speed)
    : m_Position(0.0f, 0.0f), m_Shape(shape), m_State(NodeState::Inactive), m_Size(size), m_Speed(speed), m_VelocityX(0.0f), m_VelocityY(0.0f), m_Rotation(0.0f) {
    m_MaxHP = size * 2.0f;
    m_HP = m_MaxHP;
}

Position& Node::GetPosition() {
    return m_Position;
}

const Position& Node::GetPosition() const {
    return m_Position;
}

NodeShape Node::GetShape() const {
    return m_Shape;
}

NodeState Node::GetState() const {
    return m_State;
}

float Node::GetSize() const {
    return m_Size;
}

float Node::GetSpeed() const {
    return m_Speed;
}

float Node::GetRotation() const {
    return m_Rotation;
}

float Node::GetHP() const {
    return m_HP;
}

float Node::GetMaxHP() const {
    return m_MaxHP;
}

void Node::TakeDamage(float damage) {
    if (m_State != NodeState::Active)
        return;

    m_HP -= damage;
    if (m_HP <= 0.0f) {
        m_HP = 0.0f;
        Kill();
    }
}

void Node::Spawn(float x, float y) {
    m_Position.x = x;
    m_Position.y = y;
    m_state = NodeState::Active;
    m_HP = m_MaxHP;
}

void Node::SetDirection(float dirX, float dirY) {
    m_VelocityX = dirX;
    m_VelocityY = dirY;
}

void Node::Kill() {
    m_state = NodeState::Dead;
}

void Node::Update(float deltaTime) {
    if (m_state != NodeState::Active)
        return;

    // Mișcare în direcția setată cu viteza specificată
    m_position.x += m_velocity.x * m_speed * deltaTime;
    m_position.y += m_velocity.y * m_speed * deltaTime;

    // Rotație lentă (30 grade pe secundă)
    m_rotation += 30.0f * deltaTime;
    if (m_rotation >= 360.0f) {
        m_rotation -= 360.0f;
    }
}
