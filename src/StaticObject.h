#pragma once
#include "GameObject.h"


class StaticObject : public GameObject
{
public:
    StaticObject(Model* model, Material* material, Texture* texture);
    void Render() override;
    void Update(float deltaTime) override;
};