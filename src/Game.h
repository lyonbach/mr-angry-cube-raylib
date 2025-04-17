#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "MrAngryCube.h"
#include "Miscellaneous.h"
#include "SimpleGui.h"
#include <vector>


class Game
{
public:
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
    std::vector<Enemy*> GetEnemies();

    int updateSpeed = 60;
    GameConfig* gameConfig;

private:
    std::vector<GameObject*> m_GameObjects;
    std::vector<TimedText*> m_TimedTexts;
    bool m_Initialized = false;
    float m_LastUpdateTime = GetTime();
    Menu* m_Menu;
    MrAngryCube* m_MrAngryCube;
    GameState m_GameState;
    Camera3D m_Camera = { 0 };
};