#pragma once

#include "raylib.h"
#include "raymath.h"
#include <string>


class GameObject
{
public:
    Matrix transform = MatrixIdentity();
    Model* model = nullptr;
    Texture* texture = nullptr;
    Material* material = nullptr;
    std::string objectId = "NO_ID";

    GameObject(Model* model, Material* material, Texture* texture);
    virtual ~GameObject();
    Vector3 GetPosition();
    void SetPosition(Vector3 newPosition);

    virtual void Render();
    virtual void Update(float deltaTime);
};
