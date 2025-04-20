#pragma once
#include "CameraController.h"
#include "Enemy.h"
#include "GameConfig.h"
#include "GameInfo.h"
#include "GameObject.h"
#include "Miscellaneous.h"
#include "MrAngryCube.h"
#include "SimpleGui.h"
#include <vector>

class MrAngryCube;
class CameraController;

class Game {
public:
    GameInfo gameInfo;
    std::vector<TimedText*> timedTexts;

    static Game& Get();
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

    Game(const Game&) = delete;
    void operator=(const Game&) = delete;
    std::vector<GameObject*> gameObjects;

private:
    Game();
    std::vector<TimedText*> m_TimedTexts;
    bool m_Initialized = false;
    float m_LastUpdateTime = 0.0f;
    Menu* m_Menu = nullptr;
    GameConfig* m_GameConfig = nullptr;
    MrAngryCube* m_MrAngryCube = nullptr;
    CameraController m_CamController;
    GameState m_GameState = GameState::MainMenu;
};
