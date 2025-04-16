#include "Game.h"
#include <algorithm>


Game::Game(GameConfig* config)
{
    m_Config = config;
    m_GameState = GameState::Playing;
    updateSpeed = m_Config->updateSpeed;
    TraceLog(LOG_DEBUG, "Configuring game with the update speed: %d", m_Config->updateSpeed);
    TraceLog(LOG_DEBUG, "Game configured with target FPS: %d", m_Config->targetFPS);
    SetTargetFPS(m_Config->targetFPS);
    InitWindow(m_Config->screenWidth, m_Config->screenHeight, m_Config->windowTitle);
    InitMenu();

    // SetExitKey(0);  // Disable exit key.

    // Initialize main character.
    m_MrAngryCube = new MrAngryCube( m_Config->texturePath, m_Config->shaderPath, m_Config->modelPath);
    Register(m_MrAngryCube);
    m_Initialized = true;
}

Game::~Game()
{
    for (auto& gameObject : m_GameObjects)
    {
        delete gameObject;
    }
    m_GameObjects.clear();
}

void Game::InitMenu()
{
    m_Menu = new Menu();
    m_Menu->AddItem(
        new PushButton("     Play     ",
            m_Config->screenWidth / 2,
            m_Config->screenHeight / 3,
            [this](){ m_GameState = GameState::Playing; })
    );
    m_Menu->AddItem(
        new PushButton("     Exit     ",
            m_Config->screenWidth / 2,
            m_Config->screenHeight / 3 + 50,
            [](){ exit(0); })
    );
}

void Game::SpawnEnemy(Vector2 coordinates)
{
    Register(new Enemy(m_Config->texturePath, m_Config->shaderPath, m_Config->modelPath));
}

void Game::Register(GameObject* gameObject)
{
    m_GameObjects.push_back(gameObject);
}

std::vector<Enemy*> Game::GetCollidingEnemies()
{
    std::vector<Enemy*> enemies;
    if (m_GameObjects.empty())
    {
        return enemies;
    }

    if (!m_MrAngryCube)
    {
        return enemies;
    }

    Vector2 mrAngryCubePosition = { m_MrAngryCube->transform.m12, m_MrAngryCube->transform.m14 };

    for (auto& gameObject : m_GameObjects)
    {
        if (Enemy* enemy = dynamic_cast<Enemy*>(gameObject))
        {
            Vector2 enemyPosition = { enemy->transform.m12, enemy->transform.m14 };
            if (fabs(mrAngryCubePosition.x - enemyPosition.x) < 0.1f && fabs(mrAngryCubePosition.y - enemyPosition.y) < 0.1f)
            {
                enemies.push_back(enemy);
            }
        }
    }
    return enemies;
}

void Game::Update()
{
    MrAngryCube* mrAngryCube = dynamic_cast<MrAngryCube*>(this->m_GameObjects[0]);  // FIXME MOVE TO A FUNCTION
    float deltaTime = GetTime() - m_LastUpdateTime;
    if (deltaTime < 1.0f / updateSpeed)
    {
        return;
    }
    for (auto& gameObject : m_GameObjects)
    {
        gameObject->Update(deltaTime);
    }

    // Check collisions.
    for (Enemy* enemy : GetCollidingEnemies())
    {
        if (mrAngryCube->IsFaceOnTheGround())
        {
            break;
        }
        Unregister(enemy);
    }

    m_LastUpdateTime = GetTime();
}

void Game::Unregister(GameObject* gameObject)
{
    auto it = std::remove(m_GameObjects.begin(), m_GameObjects.end(), gameObject);
    if (it != m_GameObjects.end())
    {
        m_GameObjects.erase(it);
    }
}

void Game::Render()
{
    if (!m_Initialized)
    {
        TraceLog(LOG_WARNING, "Game not initialized!");
        return;
    }

    BeginDrawing();
    ClearBackground(DARKBLUE);
    switch (m_GameState)
    {
        case GameState::MainMenu:
        m_Menu->Update();
        m_Menu->Render();
        break;

        case GameState::Playing:
        {
            // Update Game
            //----------------------------------------------------------------------------------
            Update();
            // FIXME move to a function
            m_Camera.target = (Vector3){m_MrAngryCube->transform.m12, m_MrAngryCube->transform.m13, m_MrAngryCube->transform.m14};
            m_Camera.position = (Vector3){m_Camera.target.x, m_Camera.target.y + 5, m_Camera.target.z - 20.0f};
            //----------------------------------------------------------------------------------

            // Render Game
            //----------------------------------------------------------------------------------
                BeginMode3D(m_Camera);
                DrawGrid(10, 1.0f);
                for (auto& gameObject : m_GameObjects)
                {
                    gameObject->Render();
                }
                EndMode3D();
                DrawFPS(10, 10);
            //----------------------------------------------------------------------------------
        }
        break;

        default:
        TraceLog(LOG_WARNING, "Unknown game state!");
        break;
    }
    EndDrawing();

}

int Game::Run()
{
    m_Camera.position = (Vector3){ 0.0f, 10.0f, -5.0f };  // Camera position
    m_Camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    m_Camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    m_Camera.fovy = 45.0f;                                // Camera field-of-view Y
    m_Camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    m_MrAngryCube->speed = 2.0f;

    while (!WindowShouldClose())
    {
        // Handle key events.
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_W))
        {
            m_MrAngryCube->nextRotationAxis = { 1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_S))
        {
            m_MrAngryCube->nextRotationAxis = { -1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_A))
        {
            m_MrAngryCube->nextRotationAxis = { 0.0f, 0.0f, -1.0f };
        } else if (IsKeyPressed(KEY_D))
        {
            m_MrAngryCube->nextRotationAxis = { 0.0f, 0.0f, 1.0f };
        } else if (IsKeyPressed(KEY_E))
        {
            m_MrAngryCube->nextRotationAxis = { 0.0f, -1.0f, 0.0f };
        } else if (IsKeyPressed(KEY_Q))
        {
            m_MrAngryCube->nextRotationAxis = { 0.0f, 1.0f, 0.0f };
        } else if (IsKeyPressed(KEY_R))
        {
            SpawnEnemy({0, 0});
        }
        //----------------------------------------------------------------------------------
        Update();
        Render();
    }
    return 0;
}

void Game::Exit()
{
    CloseWindow();
    exit(0);
}
