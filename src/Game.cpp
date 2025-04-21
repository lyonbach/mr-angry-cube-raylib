#include "Game.h"
#include <algorithm>


Game::Game()
{
    Utilities::Log("Created Game singleton class.", "GAME");
}

Game::~Game()
{
    for (auto& gameObject : gameObjects)
    {
        delete gameObject;
    }
    gameObjects.clear();
}

Game& Game::Get() {
    static Game instance;
    return instance;
}

void Game::Init(GameConfig* configuration)
{
    m_GameState = GameState::Playing;
    gameConfig = configuration;
    InitWindow(gameConfig->screenWidth, gameConfig->screenHeight, gameConfig->windowTitle.c_str());
    InitMenu();
    SetExitKey(0);  // Disable exit key.

    // Initialize main character.
    mrAngryCube = new MrAngryCube(
        gameConfig->texturePath.c_str(),
        gameConfig->shaderPath.c_str(),
        gameConfig->modelPath.c_str()
    );
    Register(mrAngryCube);
    m_Initialized = true;
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

void Game::SpawnEnemy()
{
    m_GameMode.spawnBehaviour();
}

void Game::Register(GameObject* gameObject)
{
    gameObjects.push_back(gameObject);
}

void Game::Unregister(GameObject* gameObject)
{
    auto it = std::remove(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}

std::vector<Enemy*> Game::GetCollidingEnemies()
{
    std::vector<Enemy*> enemies;
    if (gameObjects.empty())
    {
        return enemies;
    }

    if (!mrAngryCube)
    {
        return enemies;
    }

    Vector2 mrAngryCubePosition = { mrAngryCube->transform.m12, mrAngryCube->transform.m14 };

    for (auto& gameObject : gameObjects)
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
    if (gameObjects.empty())
    {
        return enemies;
    }
    if (!mrAngryCube)
    {
        return enemies;
    }

    for (auto& gameObject : gameObjects)
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
    // Update only if playing.
    if (m_GameState != GameState::Playing) { return; }

    float deltaTime = GetTime() - gameInfo.lastUpdateTime;
    if (deltaTime < 1.0f / gameConfig->updateSpeed) { return; }
    for (auto& gameObject : gameObjects) { gameObject->Update(deltaTime); }

    if (mrAngryCube->IsAtQuarterRotation())
    {
        const char* quote = "";
        bool shouldGetAngrier = false;
        int totalRotationCount = Utilities::SumVector3(gameInfo.rotationCount);
        if (gameInfo.angerIncrementCountdown <= 0 && totalRotationCount > 0)
        {
            // If countdown is zero and we have any rotation around any axis.
            quote = "Dizzy! Getting angry!";
            shouldGetAngrier = true;

            // We also set the countdown to its default value.
            gameInfo.angerIncrementCountdown = gameInfo.defaultAngerIncrementCountdown;
        }

        if (mrAngryCube->IsFaceOnTheGround())
        {
            quote = "My Face!";
            shouldGetAngrier = true;
        }

        // FIXME Handle quote display -> TEMPORARY.
        if (shouldGetAngrier && !mrAngryCube->isMoving)
        {
            if (Utilities::SumVector3(gameInfo.rotationCount) != gameInfo.lastRotationCount)
            {
                timedTexts.push_back(Utilities::GetTimedText(quote));
                gameInfo.anger = std::min((int)gameInfo.possibleSpeeds.size() - 1, ++gameInfo.anger);
                gameInfo.lastRotationCount = Utilities::SumVector3(gameInfo.rotationCount);
            }
        }

        for (Enemy* enemy : GetCollidingEnemies())
        {
            Unregister(enemy);
            gameInfo.score++;
            gameInfo.anger = std::max(0, --gameInfo.anger);
            gameInfo.angerIncrementCountdown = gameInfo.defaultAngerIncrementCountdown;
        }

        mrAngryCube->speed = gameInfo.possibleSpeeds.at(gameInfo.anger);
    }

    gameInfo.lastUpdateTime = GetTime();
}

void Game::Render()
{
    if (!m_Initialized)
    {
        TraceLog(LOG_WARNING, "Game not initialized!");
        return;
    }

    BeginDrawing();
    ClearBackground(gameConfig->backgroundColor);
    switch (m_GameState)
    {
        case GameState::MainMenu:
        m_Menu->Update();
        m_Menu->Render();
        break;
        case GameState::Playing:
            m_CamController.Render();
        break;
        case GameState::Paused:
        {
            // Do nothing.
        }
        case GameState::GameOver:
            ClearBackground(gameConfig->backgroundColor);
        break;
        default:
            TraceLog(LOG_ERROR, "Unknown game state!");
        break;
    }
    RenderHud();
    EndDrawing();
}

void Game::RenderHud()
{
    std::vector<std::string> textsToRender;
    switch (m_GameState)
    {
        case GameState::Playing:
        {
            // FIXME THIS IS NOT A GOOD WAY TO DO IT. THINK ABOUT THE EVENT SYSTEM.
            for(auto it=timedTexts.begin(); it!=timedTexts.end();)
            {
                auto timedText = *it;
                if(GetTime() - timedText->lastCheckTime > timedText->duration)
                {
                    it = timedTexts.erase(it);
                } else {
                    timedText->Draw();
                    it++;
                }
            }

            // Draw score. FIXME SHOULD BE IMPROVED
            int fontSize = 20;
            int percentage = (int)((float)gameInfo.anger / (gameInfo.possibleSpeeds.size() - 1) * 100);
            int rotations = gameInfo.rotationCount.x + gameInfo.rotationCount.y + gameInfo.rotationCount.z;
            textsToRender = {
                std::string("Score: " + std::to_string(gameInfo.score)),
                std::string("Anger: " + std::to_string(percentage) + "%"),
                std::string("Enemies Alive: " + std::to_string(GetEnemies().size())),
                std::string("Rotations: " + std::to_string(rotations))
            };
            for (size_t i = 0; i < textsToRender.size(); i++)
            {
                std::string text = textsToRender.at(i);
                DrawText(text.c_str(), 2*fontSize, (2 + 2 * i) * fontSize, fontSize, YELLOW);
            }
        }
        break;
        case GameState::Paused:
        {
            int fontSize = 30;
            const char* text = "GAME PAUSED";
            DrawText(text, (GetScreenWidth() - MeasureText(text, fontSize)) / 2, (GetScreenHeight() - fontSize) / 2, fontSize, WHITE);
        }
        break;
        case GameState::GameOver:
        {
            int fontSize = 30;
            const char* text = "GAME OVER";
            DrawText(text, (GetScreenWidth() - MeasureText(text, fontSize)) / 2, (GetScreenHeight() - fontSize) / 2, fontSize, WHITE);
        }
        break;

    }
}

int Game::Run()
{
    m_CamController.Run(mrAngryCube);
    while (!WindowShouldClose())  // Main loop.
    {
        HandleKeyEvents();
        Update();
        Render();
    }
    return 0;
}

void Game::HandleKeyEvents()
{
    if (IsKeyPressed(KEY_W))
    {
        mrAngryCube->nextRotationAxis = m_CamController.GetRightVector();
    } else if (IsKeyPressed(KEY_S))
    {
        mrAngryCube->nextRotationAxis = m_CamController.GetLeftVector();
    } else if (IsKeyPressed(KEY_A))
    {
        mrAngryCube->nextRotationAxis = m_CamController.GetRearVector();
    } else if (IsKeyPressed(KEY_D))
    {
        mrAngryCube->nextRotationAxis = m_CamController.GetFrontVector();
    } else if (IsKeyPressed(KEY_E))
    {
        mrAngryCube->nextRotationAxis = { 0.0f, -1.0f, 0.0f };
    } else if (IsKeyPressed(KEY_Q))
    {
        mrAngryCube->nextRotationAxis = { 0.0f, 1.0f, 0.0f };
    } else if (IsKeyPressed(KEY_R))
    {
        SpawnEnemy();
    } else if (IsKeyPressed(KEY_ESCAPE)) {
        switch (m_GameState)
        {
            case GameState::Playing:
                m_GameState = GameState::Paused;
            break;

            case GameState::Paused:
                m_GameState = GameState::Playing;
            break;

            case GameState::GameOver:
                m_GameState = GameState::MainMenu;

            default:
            break;
        }
    } else if (IsKeyPressed(KEY_RIGHT)) {
        m_CamController.RotateCamera(RotationDirection::CW);
    } else if (IsKeyPressed(KEY_LEFT)) {
        m_CamController.RotateCamera(RotationDirection::CCW);
    } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP)) {
        m_CamController.RotateCamera(RotationDirection::CCW);
        m_CamController.RotateCamera(RotationDirection::CCW);
    }
}

void Game::Exit()
{
    CloseWindow();
    exit(0);
}
