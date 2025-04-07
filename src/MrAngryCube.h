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

        MrAngryCube(const char* texturePath, const char* shaderPath, const char* modelPath);
        virtual ~MrAngryCube() = default;

        void Render() override;
        using GameObject::Update;
        void Update(Vector3 rotationAxis, float increment);
};

