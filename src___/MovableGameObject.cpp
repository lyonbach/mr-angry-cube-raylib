#include "MovableGameObject.h"


MovableGameObject::MovableGameObject(Model& model, Shader& shader, Texture& texture)
    : GameObject(model, shader, texture)
{
    transform = MatrixScale(size.x, size.y, size.z);
}

void MovableGameObject::Render()
{

}

void MovableGameObject::Update(float deltaTime)
{

}