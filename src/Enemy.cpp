#include "Enemy.h"


Enemy::Enemy(const char* texturePath, const char* shaderPath, const char* modelPath)
    : GameObject(texturePath, shaderPath, modelPath)
{
}

void Enemy::Render()
{
    Vector3 position = { m_Transform.m12, m_Transform.m13, m_Transform.m14 };
    DrawCube(position, 1, 1, 1, WHITE);
    
}

void Enemy::Update()
{
}