#include "Enemy.h"


Enemy::Enemy(Model& model, Shader& shader, Texture& texture, float size, Color color)
    : GameObject(model, shader, texture)
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

void Enemy::SetPosition(Vector3 position)
{
    transform = MatrixMultiply(MatrixTranslate(position.x, m_Size / 2, position.z), transform);
}
