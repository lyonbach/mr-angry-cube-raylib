#include "Game.h"
#include <algorithm>


Game::Game(GameConfig* config)
{
    gameConfig = config;
    m_GameState = GameState::Playing;
    updateSpeed = gameConfig->updateSpeed;
    TraceLog(LOG_DEBUG, "Configuring game with the update speed: %d", gameConfig->updateSpeed);
    TraceLog(LOG_DEBUG, "Game configured with target FPS: %d", gameConfig->targetFPS);
    SetTargetFPS(gameConfig->targetFPS);
    InitWindow(gameConfig->screenWidth, gameConfig->screenHeight, gameConfig->windowTitle);
    InitMenu();

    // SetExitKey(0);  // Disable exit key.

    // Initialize main character.
    m_MrAngryCube = new MrAngryCube( gameConfig->texturePath, gameConfig->shaderPath, gameConfig->modelPath);
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
            gameConfig->screenWidth / 2,
            gameConfig->screenHeight / 3,
            [this](){ m_GameState = GameState::Playing; })
    );
    m_Menu->AddItem(
        new PushButton("     Exit     ",
            gameConfig->screenWidth / 2,
            gameConfig->screenHeight / 3 + 50,
            [](){ exit(0); })
    );
}

void Game::SpawnEnemy(Vector2 coordinates)
{
    // Enemies can appear next to the Mr. Angry Cube or 20 unit distance from him.
    // In addition, each coordinate should be divisible to 2.
    int randX = GetRandomValue(2, 20);
    int randZ = GetRandomValue(2, 20);
    if (randX % 2 != 0) randX++;
    if (randZ % 2 != 0) randZ++;

    Enemy* enemy = new Enemy(gameConfig->texturePath, gameConfig->shaderPath, gameConfig->modelPath);
    enemy->SetPosition({(float)randX, (float)randZ});
    Register(enemy);
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

std::vector<Enemy*> Game::GetEnemies()
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

    for (auto& gameObject : m_GameObjects)
    {
        if (Enemy* enemy = dynamic_cast<Enemy*>(gameObject))
        {
            enemies.push_back(enemy);
        }
    }
    return enemies;
}

void Game::Update()
{
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
        if (m_MrAngryCube->IsFaceOnTheGround())
        {
            break;
        }
        Unregister(enemy);
        m_MrAngryCube->gameInfo.score++;
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
            // FIXME move to a function
            m_Camera.target = (Vector3){m_MrAngryCube->transform.m12, m_MrAngryCube->transform.m13, m_MrAngryCube->transform.m14};
            m_Camera.position = (Vector3){m_Camera.target.x, m_Camera.target.y + 5, m_Camera.target.z - 20.0f};
            //----------------------------------------------------------------------------------

            // Render Game
            //----------------------------------------------------------------------------------
                BeginMode3D(m_Camera);
                DrawGrid(200, 1.0f);
                for (auto& gameObject : m_GameObjects) { gameObject->Render(); }
                EndMode3D();
                DrawFPS(10, 10);
            //----------------------------------------------------------------------------------
        }
        break;

        default:
        TraceLog(LOG_WARNING, "Unknown game state!");
        break;
    }

    // FIXME THIS IS NOT A GOOD WAY TO DO IT. THINK ABOUT THE EVENT SYSTEM.
    for(auto it=m_MrAngryCube->timedTexts.begin(); it!=m_MrAngryCube->timedTexts.end();)
    {
        auto timedText = *it;
        if(GetTime() - timedText->lastCheckTime > timedText->duration)
        {
            it = m_MrAngryCube->timedTexts.erase(it);
        } else {
            timedText->Draw();
            it++;
        }
    }

    // Draw score. FIXME SHOULD BE IMPROVED
    int fontSize = 30;
    std::string scoreText ("Score: " + std::to_string(m_MrAngryCube->gameInfo.score));
    DrawText(scoreText.c_str(), 2*fontSize, GetScreenHeight() - 6*fontSize, fontSize, YELLOW);
    std::string angerText ("Anger : " + std::to_string(m_MrAngryCube->gameInfo.anger / m_MrAngryCube->gameInfo.maxAnger * 100.0f) + "%");
    DrawText(angerText.c_str(), 2*fontSize, GetScreenHeight() - 4*fontSize, fontSize, YELLOW);
    std::string enemyCountText ("Enemies Alive: " + std::to_string(GetEnemies().size()));
    DrawText(enemyCountText.c_str(), 2*fontSize, GetScreenHeight() - 2*fontSize, fontSize, YELLOW);
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
