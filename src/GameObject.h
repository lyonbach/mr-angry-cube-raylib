#pragma once

#include "raylib.h"
#include "raymath.h"
#include <string>
#include <vector>


class GameObject
{
public:
    Matrix transform = MatrixIdentity();
    Model* model = nullptr;
    std::vector<Material*> materials;
    std::string objectId = "NO_ID";

    GameObject(Model* model, std::vector<Material*> materials);
    virtual ~GameObject();
    Vector3 GetPosition();
    void SetPosition(Vector3 newPosition);

    virtual void Render();
    virtual void Update(float deltaTime);
};
