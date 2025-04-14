#include "Game.h"
#include <algorithm>


Game::Game(GameConfig* config)
{
    m_Config = config;
    m_GameState = GameState::MainMenu;
    m_UpdateSpeed = m_Config->m_UpdateSpeed;
    TraceLog(LOG_DEBUG, "Configuring game with the update speed: %d", m_Config->m_UpdateSpeed);
    TraceLog(LOG_DEBUG, "Game configured with target FPS: %d", m_Config->m_TargetFPS);
    SetTargetFPS(m_Config->m_TargetFPS);
    InitWindow(m_Config->m_ScreenWidth, m_Config->m_ScreenHeight, m_Config->m_WindowTitle);
    InitMenu();

    // SetExitKey(0);  // Disable exit key.

    // Initialize main character.
    Register( new MrAngryCube( m_Config->m_TexturePath, m_Config->m_ShaderPath, m_Config->m_ModelPath) );

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
            m_Config->m_ScreenWidth / 2,
            m_Config->m_ScreenHeight / 3,
            [this](){ m_GameState = GameState::Playing; })
    );
    m_Menu->AddItem(
        new PushButton("     Exit     ",
            m_Config->m_ScreenWidth / 2,
            m_Config->m_ScreenHeight / 3 + 50,
            [](){ exit(0); })
    );
}

void Game::SpawnEnemy(Vector2 coordinates)
{
    Register(new Enemy(m_Config->m_TexturePath, m_Config->m_ShaderPath, m_Config->m_ModelPath));
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

    MrAngryCube* mrAngryCube = dynamic_cast<MrAngryCube*>(m_GameObjects[0]);
    if (!mrAngryCube)
    {
        return enemies;
    }

    Vector2 mrAngryCubePosition = { mrAngryCube->m_Transform.m12, mrAngryCube->m_Transform.m14 };

    for (auto& gameObject : m_GameObjects)
    {
        if (Enemy* enemy = dynamic_cast<Enemy*>(gameObject))
        {
            Vector2 enemyPosition = { enemy->m_Transform.m12, enemy->m_Transform.m14 };
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
    float deltaTime = GetTime() - m_LastUpdateTime;
    if (deltaTime < 1.0f / m_UpdateSpeed)
    {
        return;
    }
    for (auto& gameObject : m_GameObjects)
    {
        gameObject->Update(deltaTime);
    }
    m_LastUpdateTime = GetTime();

    MrAngryCube* mrAngryCube = dynamic_cast<MrAngryCube*>(this->m_GameObjects[0]);  // FIXME MOVE TO A FUNCTION
    if ( // 90 -> quarter rotation.
        (int)mrAngryCube->m_Rotation.x % 90 == 0 && mrAngryCube->m_RotationAxis.x != 0.0f ||
        (int)mrAngryCube->m_Rotation.z % 90 == 0 && mrAngryCube->m_RotationAxis.z != 0.0f ||
        (int)mrAngryCube->m_Rotation.y % 90 == 0 && mrAngryCube->m_RotationAxis.y != 0.0f ||
        (mrAngryCube->m_RotationAxis.x == 0.0f &&
         mrAngryCube->m_RotationAxis.z == 0.0f &&
         mrAngryCube->m_RotationAxis.y == 0.0f)
        )
    {
        mrAngryCube->m_RotationAxis = mrAngryCube->m_NextRotationAxis;

        // Check collisions.
        for (Enemy* enemy : GetCollidingEnemies())
        {
            if (mrAngryCube->IsFaceOnTheGround())
            {
                break;
            }
            Unregister(enemy);
        }
    }

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

    for (auto& gameObject : m_GameObjects)
    {
        gameObject->Render();
    }
}

int Game::Run()
{
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, -5.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    MrAngryCube* mrAngryCube = dynamic_cast<MrAngryCube*>(this->m_GameObjects[0]);  // FIXME MOVE TO A FUNCTION
    mrAngryCube->m_Speed = 2.0f;

    while (!WindowShouldClose())
    {
        // Handle key events.
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_W))
        {
            mrAngryCube->m_NextRotationAxis = { 1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_S))
        {
            mrAngryCube->m_NextRotationAxis = { -1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_A))
        {
            mrAngryCube->m_NextRotationAxis = { 0.0f, 0.0f, -1.0f };
        } else if (IsKeyPressed(KEY_D))
        {
            mrAngryCube->m_NextRotationAxis = { 0.0f, 0.0f, 1.0f };
        } else if (IsKeyPressed(KEY_E))
        {
            mrAngryCube->m_NextRotationAxis = { 0.0f, -1.0f, 0.0f };
        } else if (IsKeyPressed(KEY_Q))
        {
            mrAngryCube->m_NextRotationAxis = { 0.0f, 1.0f, 0.0f };
        } else if (IsKeyPressed(KEY_R))
        {
            SpawnEnemy({0, 0});
        }
        //----------------------------------------------------------------------------------

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
                camera.target = (Vector3){mrAngryCube->m_Transform.m12, mrAngryCube->m_Transform.m13, mrAngryCube->m_Transform.m14};
                camera.position = (Vector3){camera.target.x, camera.target.y + 5, camera.target.z - 20.0f};
                //----------------------------------------------------------------------------------

                // Render Game
                //----------------------------------------------------------------------------------
                    BeginMode3D(camera);
                        Render();
                        DrawGrid(10, 1.0f);
                    EndMode3D();
                    DrawFPS(10, 10);
                    DrawText(("Target Update Rate: " + std::to_string(m_UpdateSpeed)).c_str(), 10, 30, 20, WHITE);
                    DrawText(("DeltaTime: " + std::to_string(std::round((GetTime() - m_LastUpdateTime) * 1000) / 1000)).c_str(), 10, 50, 20, WHITE);
                //----------------------------------------------------------------------------------
            }
            break;

            default:
            TraceLog(LOG_WARNING, "Unknown game state!");
            break;
        }
        EndDrawing();
    }
    return 0;
}

void Game::Exit()
{
    CloseWindow();
    exit(0);
}
