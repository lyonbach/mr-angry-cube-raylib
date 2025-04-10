#pragma once

#include "raylib.h"
#include "raymath.h"

class GameObject
{
    protected:
        Model m_Model;
        Texture m_Texture;
        Shader m_Shader;
        Material m_Material;

    public:

        Matrix m_Transform;

        GameObject(const char* texturePath, const char* shaderPath, const char* modelPath);
        ~GameObject();

        virtual void Render();
        virtual void Update();
};
