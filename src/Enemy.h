#pragma once

#include "GameObject.h"

class Enemy : public GameObject
{
public:
    Enemy(Model& model, Shader& shader, Texture& texture, float size=1.0f, Color color=RED);
    virtual ~Enemy() = default;
    void Render() override;
    void Update(float deltaTime) override;
    void SetPosition(Vector3 position) override;

private:
    Color m_Color;
    float m_Size;
};


