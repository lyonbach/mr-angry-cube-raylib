#pragma once

#include "GameObject.h"


class LevelObject : public GameObject
{
public:
    LevelObject(Model& model, Shader& shader, Texture& texture, float size=1.0f, Color color=RED);
    void Render() override;
};