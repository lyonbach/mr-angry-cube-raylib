#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
    public:

        Color m_Color;
        Enemy(const char* texturePath, const char* shaderPath, const char* modelPath, Color color=RED);
        virtual ~Enemy() = default;
        
        void Render() override;
        void Update(float deltaTime) override;

};


