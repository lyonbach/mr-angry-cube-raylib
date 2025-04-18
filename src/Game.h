#pragma once
#include "Enemy.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Miscellaneous.h"
#include "MrAngryCube.h"
#include "SimpleGui.h"
#include <vector>


class Game
{
public:
    static Game& Get();
    ~Game();

    void Init();
    void SpawnEnemy(Vector2 coordinates);
    void Register(GameObject* gameObject);
    void Unregister(GameObject* gameObject);
    void InitMenu();
    void Update();
    void Render();
    void RenderHud();
    int Run();
    void Exit();
    std::vector<Enemy*> GetCollidingEnemies();
    std::vector<Enemy*> GetEnemies();
    int updateSpeed;
    static Game s_Game;

    Game(Game &other) = delete;
    void operator=(const Game &) = delete;
    
private:
    Game();
    std::vector<GameObject*> m_GameObjects;
    std::vector<TimedText*> m_TimedTexts;
    bool m_Initialized = false;
    float m_LastUpdateTime = GetTime();
    Menu* m_Menu;
    MrAngryCube* m_MrAngryCube;
    GameState m_GameState;
    Camera3D m_Camera = { 0 };
};