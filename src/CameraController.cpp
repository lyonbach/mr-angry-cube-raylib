#include "CameraController.h"
#include "Game.h"
#include "MrAngryCube.h"


CameraController::CameraController()
{
    camera = new Camera();
    camera->target = Vector3();
    camera->position = chaseVector;
    camera->up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera->fovy = 70.0f;
    camera->projection = CAMERA_PERSPECTIVE;
}

CameraController::~CameraController()
{
    delete camera;
}

void CameraController::Update(float deltaTime)
{
    Vector3 distanceVector = Game::Get().GetPlayer()->GetPosition() - camera->target;
    camera->target += distanceVector * deltaTime;
    camera->position = camera->target + chaseVector;
}