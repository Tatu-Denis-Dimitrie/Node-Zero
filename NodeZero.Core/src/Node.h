#pragma once

#include "../include/INode.h"

class Node : public INode {
   private:
    Position m_Position;
    NodeShape m_Shape;
    NodeState m_State;
    float m_Size;
    float m_Speed;
    float m_HP;
    float m_MaxHP;
    float m_VelocityX;
    float m_VelocityY;
    float m_Rotation;

   public:
    Node(NodeShape shape, float size, float speed);

    Position& GetPosition() override;
    const Position& GetPosition() const override;

    NodeShape GetShape() const override;
    NodeState GetState() const override;

    float GetSize() const override;
    float GetSpeed() const override;
    float GetRotation() const override;

    float GetHP() const override;
    float GetMaxHP() const override;

    void Spawn(float x, float y) override;
    void SetDirection(float dirX, float dirY) override;
    void Update(float deltaTime) override;
    void Kill() override;
    void TakeDamage(float damage) override;

    void SetHP(float hp);
};
