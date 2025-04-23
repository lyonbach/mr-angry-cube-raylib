#pragma once

#include "raylib.h"
#include "raymath.h"


class GameObject
{
public:
    Matrix transform;

    GameObject(const char* texturePath, const char* shaderPath, const char* modelPath);
    ~GameObject();
    virtual void Render();
    virtual void Update(float lastUpdateTime);
    virtual void SetPosition(Vector3 position);
    virtual Vector3 GetPosition();
    virtual Vector3 GetVelocity(float deltaTime);


protected:
    Model m_Model;
    Texture m_Texture;
    Shader m_Shader;
    Material m_Material;
    Vector3 m_LastPosition;
};
