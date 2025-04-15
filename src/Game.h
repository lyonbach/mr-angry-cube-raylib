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
    
    Game(GameConfig* config);
    ~Game();
    
    void SpawnEnemy(Vector2 coordinates);
    void Register(GameObject* gameObject);
    void Unregister(GameObject* gameObject);
    void InitMenu();
    
    void Update();
    void Render();
    int Run();
    void Exit();
    std::vector<Enemy*> GetCollidingEnemies();
    
    int updateSpeed = 60;

private:
    bool m_Initialized = false;
    float m_LastUpdateTime;
    Menu* m_Menu;
    GameConfig* m_Config;
    MrAngryCube* m_MrAngryCube;
    GameState m_GameState;
    GameInfo m_GameInfo;
    Camera3D m_Camera = { 0 };
};