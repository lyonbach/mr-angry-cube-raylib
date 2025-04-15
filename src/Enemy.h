#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
public:
    Enemy(const char* texturePath, const char* shaderPath, const char* modelPath, Color color=RED);
    virtual ~Enemy() = default;
    void Render() override;
    void Update(float deltaTime) override;

private:
    Color m_Color;
};


