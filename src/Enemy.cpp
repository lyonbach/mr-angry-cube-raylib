#include "Enemy.h"


Enemy::Enemy(const char* texturePath, const char* shaderPath, const char* modelPath, Color color)
    : GameObject(texturePath, shaderPath, modelPath)
{
    m_Color = color;
}

void Enemy::Render()
{
    Vector3 position = { m_Transform.m12, m_Transform.m13, m_Transform.m14 };
    DrawCube(position, 1, 1, 1, m_Color);
    
}

void Enemy::Update(float deltaTime)
{
}