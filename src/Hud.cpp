#include "Hud.h"
#include "Game.h"
#include "MrAngryCube.h"


void Hud::Render()
{
    DrawFPS(50, 50);


    int fontSize = (int)(GetScreenHeight() / 25.0f);

    MrAngryCube* player = Game::Get().GetPlayer();
    std::string text;
    
    Vector3 rotation = player->rotation;
    Vector3 velocity = Game::Get().physicsObserver->GetVelocity();
    Vector3 position = player->GetPosition();
    float anger = Game::Get().GetPlayer()->GetAnger();
    
    DrawText(("Anger to Behaviour Inc.: " + std::to_string(anger)).c_str(), 50, GetScreenHeight() - 250, fontSize, YELLOW);
    DrawText(("Rot: " + Hud::Vector3ToString(rotation)).c_str(), 50, GetScreenHeight() - 200, fontSize, YELLOW);
    DrawText(("Vel: " + Hud::Vector3ToString(velocity)).c_str(), 50, GetScreenHeight() - 150, fontSize, YELLOW);
    DrawText(("Pos: " + Hud::Vector3ToString(position)).c_str(), 50, GetScreenHeight() - 100, fontSize, YELLOW);
}

std::string Hud::Vector3ToString(Vector3& vector)
{
    return std::to_string(vector.x) + ", " + 
           std::to_string(vector.y) + ", " + 
           std::to_string(vector.z);
}
