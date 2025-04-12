#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
    public:
        Enemy(const char* texturePath, const char* shaderPath, const char* modelPath);
        virtual ~Enemy() = default;
        
        void Render() override;
        void Update(float deltaTime) override;

};


