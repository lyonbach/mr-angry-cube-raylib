#pragma once

#include "GameObject.h"

#include "raylib.h"
#include "raymath.h"


class MrAngryCube : public GameObject
{
    public:
        Vector3 m_Rotation;
        float m_Size;
        float m_HalfSize;
        float m_Hypotenuse;
        float m_AngerLevel = 1.0f;  // Mr. Angry Cube is least a little angry.
        float m_Speed      = 1.0f;  // Degrees per update.
        Vector3 m_RotationAxis     = { 0.0f, 0.0f, 0.0f };
        Vector3 m_NextRotationAxis = { 0.0f, 0.0f, 0.0f };

        MrAngryCube(const char* texturePath, const char* shaderPath, const char* modelPath);
        virtual ~MrAngryCube() = default;

        void Render() override;
        void Update(float deltaTime) override;
        bool IsFaceOnTheGround();
};

