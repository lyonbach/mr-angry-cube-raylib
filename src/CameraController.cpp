#include "CameraController.h"
#include "Game.h"
#include "MrAngryCube.h"


CameraController::CameraController()
{
    camera = new Camera();
    camera->target = Vector3();
    camera->position = chaseVector;
    camera->up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera->fovy = 80.0f;
    camera->projection = CAMERA_PERSPECTIVE;
}

CameraController::~CameraController()
{
    delete camera;
}

void CameraController::Update(float deltaTime)
{
    MrAngryCube* player = Game::Get().GetPlayer();
    nextPositon = camera->target + chaseVector;
    Vector3 distanceVectorTarget = player->GetPosition() - camera->target;
    camera->target += distanceVectorTarget * deltaTime * CAMERA_TARGET_UPDATE_SPEED_COEFF;
    

    if(player->IsAtQuarterRotation(player->rotation) && player->canMove)
    {
        cameraShakeStrenght = STANDARD_CAMERA_SHAKE_STRENGTH;
        if ((int)abs(Game::Get().currentRotationAxis.y == 1))
        {
            cameraShakeStrenght = 0.05f;
        }
    }
    nextPositon.x += cameraShakeStrenght * GetRandomValue(-1, 1);
    nextPositon.y += cameraShakeStrenght * GetRandomValue(-1, 1);
    nextPositon.z += cameraShakeStrenght * GetRandomValue(-1, 1);
    cameraShakeStrenght *= .8f;

    Vector3 distanceVectorPosition = nextPositon - camera->position;
    camera->position += distanceVectorPosition * deltaTime * CAMERA_UPDATE_SPEED_COEFF;

}

void CameraController::ZoomIn()
{
    Utilities::Log("Zoom in requested...", "CameraController", LOG_DEBUG);
    Vector3 targetChaseVector = Vector3Add(Vector3Scale(GetFrontVector(), CAMERA_CHASE_VECTOR_INCREMENT), chaseVector);
    if(Vector3Length(GetFrontVector() * targetChaseVector) < MIN_CAMERA_CHASE_DISTANCE) { return; }
    chaseVector = targetChaseVector;
}

void CameraController::ZoomOut()
{
    Utilities::Log("Zoom out requested...", "CameraController", LOG_DEBUG);
    Vector3 targetChaseVector = Vector3Add(Vector3Scale(GetFrontVector(), -CAMERA_CHASE_VECTOR_INCREMENT), chaseVector);
    if(Vector3Length(GetFrontVector() * targetChaseVector) > MAX_CAMERA_CHASE_DISTANCE) { return; }
    chaseVector = targetChaseVector;
}

void CameraController::LeftView()
{
    Utilities::Log("Left view requested...", "CameraController", LOG_DEBUG);
    Matrix rotation = MatrixRotateY(-PI / 2);
    chaseVector = Vector3Transform(chaseVector, rotation);
}

void CameraController::RightView()
{
    Utilities::Log("Right view requested...", "CameraController", LOG_DEBUG);
    Matrix rotation = MatrixRotateY(PI / 2);
    chaseVector = Vector3Transform(chaseVector, rotation);
}

void CameraController::MoveUp()
{
    Utilities::Log("Moving camera up...", "CameraController", LOG_DEBUG);
    cameraVerticalValueIndex = std::min(2, cameraVerticalValueIndex+1);
    chaseVector.y = possibleCameraVerticalValues.at(cameraVerticalValueIndex);
}

void CameraController::MoveDown()
{
    Utilities::Log("Moving camera down...", "CameraController", LOG_DEBUG);
    cameraVerticalValueIndex = std::max(0, cameraVerticalValueIndex-1);
    chaseVector.y = possibleCameraVerticalValues.at(cameraVerticalValueIndex);
}

Vector3 CameraController::GetFrontVector()
{
    return Vector3Normalize(Vector3Multiply(chaseVector, {-1, 0, -1}));
}

Vector3 CameraController::GetBackVector()
{
    return Vector3Normalize(Vector3Multiply(chaseVector, {1, 0, 1}));
}

Vector3 CameraController::GetRightVector()
{
    Vector3 upVector = {0.0f, 1.0f, 0.0f};
    return Vector3CrossProduct(upVector, GetFrontVector());
}

Vector3 CameraController::GetLeftVector()
{
    Vector3 upVector = {0.0f, 1.0f, 0.0f};
    return Vector3CrossProduct(GetFrontVector(), upVector);
}

