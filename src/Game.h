#pragma once
#include "Enemy.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Miscellaneous.h"
#include "MrAngryCube.h"
#include "SimpleGui.h"
#include <vector>

class MrAngryCube;  // Forward declare

class Game {
public:
    static Game& Get();  // Singleton accessor
    ~Game();

    void Init(GameConfig* configuration);
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
    
    GameInfo gameInfo;
    std::vector<TimedText*> timedTexts;

    // Prevent copy
    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

private:
    Game();  // Private constructor for singleton
    std::vector<GameObject*> m_GameObjects;
    std::vector<TimedText*> m_TimedTexts;
    bool m_Initialized = false;
    float m_LastUpdateTime = 0.0f;
    Menu* m_Menu = nullptr;
    GameConfig* m_GameConfig = nullptr;
    MrAngryCube* m_MrAngryCube = nullptr;
    GameState m_GameState;
    Camera3D m_Camera = { 0 };
};
