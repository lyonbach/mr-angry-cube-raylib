#pragma once

#include "GameObject.h"

#include "raylib.h"
#include "raymath.h"


class MrAngryCube : public GameObject
{
    
public:
    MrAngryCube(const char* texturePath, const char* shaderPath, const char* modelPath);
    virtual ~MrAngryCube() = default;
    
    void Render() override;
    void Update(float deltaTime) override;
    bool IsFaceOnTheGround();
    Vector3 rotation;
    Vector3 nextRotationAxis = { 0.0f, 0.0f, 0.0f };
    Vector3 rotationAxis = { 0.0f, 0.0f, 0.0f };
    float speed = 1.0f;  // Degrees per update.

private:
    float m_Size;
    float m_HalfSize;
    float m_Hypotenuse;
    float m_AngerLevel = 1.0f;  // Mr. Angry Cube is least a little angry.
};

