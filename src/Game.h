#pragma once

#include "Enemy.h"
#include "GameObject.h"
#include "MrAngryCube.h"

#include <vector>

class Game
{

public:
    std::vector<GameObject*> m_GameObjects;
    bool m_CanQuit = false;
    float m_LastUpdateTime;
    int m_UpdateSpeed = 60;

    Game();
    Game(std::vector<GameObject*> gameObjects);
    void Register(GameObject* gameObject);
    ~Game();

    void Update();
    void Render();
    std::vector<Enemy*> GetCollidingEnemies();
};