#include "Enemy.h"


Enemy::Enemy(const char* texturePath, const char* shaderPath, const char* modelPath, float size, Color color)
    : GameObject(texturePath, shaderPath, modelPath)
{
    m_Size = size;
    m_Color = color;
}

void Enemy::Render()
{
    Vector3 position = { transform.m12, transform.m13, transform.m14 };
    DrawCube(position, m_Size, m_Size, m_Size, m_Color);
    
}

void Enemy::Update(float deltaTime)
{
    transform.m13 = m_Size / 2;
}

void Enemy::SetPosition(Vector2 position)
{
    transform = MatrixMultiply(MatrixTranslate(position.x, m_Size / 2, position.y), transform);
}