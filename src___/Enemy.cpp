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

}

void Enemy::Update_(float deltaTime, GameObject* other)
{
    switch (state)
    {
    case GameObjectState::Alive:
{
        // Calculate the direction vector (XZ-plane only)
        Vector3 direction = Vector3Subtract(other->GetPosition(), GetPosition());
        direction.y = 0.0f; // Ignore the Y-axis for rotation
        direction = Vector3Normalize(direction);

        // Calculate the angle to rotate around the Y-axis
        float angle = atan2(direction.x, direction.z); // Angle in radians

        // Create a rotation matrix for the Y-axis
        Matrix rotation = MatrixRotateY(angle);

        Vector3 position = GetPosition();

        // Apply the rotation to the transform matrix
        transform = MatrixMultiply(rotation, MatrixIdentity());

        transform.m12 = position.x;
        transform.m13 = position.y;
        transform.m14 = position.z;

        // transform = MatrixMultiply(MatrixRotateY(DEG2RAD * 1.0f), transform);
    }
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