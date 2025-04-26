#include "Enemy.h"


Enemy::Enemy(Model& model, Shader& shader, Texture& texture, float size, Color color)
    : GameObject(model, shader, texture)
{
    m_Size = size / 2.0f;
    m_Color = color;
    transform = MatrixScale(m_Size, m_Size, m_Size);
}

void Enemy::Render()
{
    DrawMesh(m_Model.meshes[0], m_Material, transform);
}

void Enemy::Update(float deltaTime)
{
    switch (state)
    {
    case GameObjectState::Alive:
        transform = MatrixMultiply(MatrixRotateY(DEG2RAD * 1.0f), transform);
        break;

    case GameObjectState::Dead:
        break;

    default:
        break;
    }
}

void Enemy::SetPosition(Vector3 position)
{
    transform.m12 = position.x;
    transform.m13 = position.y;
    transform.m14 = position.z;
}

void Enemy::SetState(GameObjectState newState)
{
    switch (newState)
    {

    case GameObjectState::Alive:
        transform.m0 = m_Size;
        transform.m5  = m_Size;
        transform.m10 = m_Size;
        transform.m13 = m_Size / 2.0f;
        break;
    
    case GameObjectState::Dead:
        transform.m0 = 2.0f * m_Size;
        transform.m5  = 0.2f * m_Size;
        transform.m10 = 2.0f * m_Size;
        transform.m13 = 0.0f * m_Size;
        deathTime = GetTime();
        break;
    
    default:
        break;
    }
    state = newState;
}