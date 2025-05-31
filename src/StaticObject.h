#pragma once
#include "GameObject.h"


class StaticObject : public GameObject
{
public:
    StaticObject(Model* model, std::vector<Material*> materials);
    void Render() override;
    void Update(float deltaTime) override;
};