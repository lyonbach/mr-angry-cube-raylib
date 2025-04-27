#pragma once

#include "GameObject.h"

class MovableGameObject : public GameObject
{
public:
    MovableGameObject(Model& model, Shader& shader, Texture& texture);
    virtual ~MovableGameObject() = default;
    void Render() override;
    void Update(float deltaTime) override;

    Model model;
    Texture texture;
    Shader shader;
    Material material;
    Vector3 size;
};