#pragma once

#include "Enemy.h"
#include "GameObject.h"
#include "MrAngryCube.h"
#include "Miscellaneous.h"
#include "SimpleGui.h"

#include <vector>

class Game
{

public:
    std::vector<GameObject*> m_GameObjects;
    Menu* m_Menu;
    GameConfig* m_Config;
    GameState m_GameState;
    bool m_Initialized = false;
    float m_LastUpdateTime;
    int m_UpdateSpeed = 60;

    Game(GameConfig* config);
    ~Game();

    void Register(GameObject* gameObject);
    void Unregister(GameObject* gameObject);
    void InitMenu();

    void Update();
    void Render();
    int Run();
    void Exit();
    std::vector<Enemy*> GetCollidingEnemies();
};