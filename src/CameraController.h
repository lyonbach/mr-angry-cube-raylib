#pragma once
#include "GameObject.h"
#include "raylib.h"

class CameraController
{
public:
    bool shouldRun = true;
    bool canUpdate = true;

    CameraController();
    void Update(GameObject* targetObject=nullptr);
    void Run(GameObject* targetObject=nullptr);
    void Render();
    private:
    Camera3D m_Camera;
    void DoCameraShake(float duration, float strength, GameObject* targetObject);
};