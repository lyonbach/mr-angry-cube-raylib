#include "MrAngryCube.h"
#include "Game.h"


Game::Game()
{
    Utilities::Log("Created Game singleton class.", "GAME");
}

Game::~Game()
{
    for (GameObject* gameObject : gameObjects)
    {
        Utilities::Log("Deleting GameObject with id: " + gameObject->objectId, "GAME");
        delete gameObject;
    }
    gameObjects.clear();
}

Game& Game::Get() {
    static Game instance;
    return instance;
}

void Game::Init(GameConfig* gameConfig)
{
    gameConfig = gameConfig;
    // SetConfigFlags(FLAG_WINDOW_OPENGL_CORE_PROFILE);
    InitWindow(gameConfig->screenSize.x, gameConfig->screenSize.y, "Mr. Angry Cube (DEV)");
    // SetExitKey(0);  // Disable exit key.

    Utilities::Log("Loading models...", "GAME");  // Models.
    for (std::pair<std::string, std::string> pair : gameConfig->modelPaths)
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "GAME");
        models[pair.first] = LoadModel(pair.second.c_str());
    }

    Utilities::Log("Loading shaders...", "GAME");
    for (std::pair<std::string, std::string> pair : gameConfig->shaderPaths)  // Shaders.
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "GAME");

        std::string vertexShaderPath = pair.second.substr(0, pair.second.find('|'));
        std::string fragmentShaderPath = pair.second.substr(pair.second.find('|') + 1);
        shaders[pair.first] = LoadShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
        
        Utilities::Log("Material created for shader: " + pair.first, "GAME");
        Material material = LoadMaterialDefault();
        material.shader = shaders[pair.first];
        materials[pair.first] = material;
    }

    Utilities::Log("Loading textures...", "GAME");
    for (std::pair<std::string, std::string> pair : gameConfig->texturePaths) // Textures.
    {
        Utilities::Log("Loading: " + pair.first + " from:\n" + pair.second, "GAME");
        textures[pair.first] = LoadTexture(pair.second.c_str());
    }

    MrAngryCube* player = new MrAngryCube(&models["macDefault"], &materials["macDefault"], &textures["macDefault"]);
    Register(player);

    m_Initialized = true;
}

void Game::Register(GameObject* newGameObject)
{
    for (GameObject* gameObject : gameObjects)
    {
        if (gameObject->objectId == newGameObject->objectId)
        {
            Utilities::Log("Can not register object with id \"" + newGameObject->objectId + "\"!\nAlready registered.", "GAME", LOG_ERROR);
            return;
        }
    }
    gameObjects.push_back(newGameObject);
}

void Game::Render()
{
    switch (gameState)
    {
    case GameState::Playing:
        
        BeginDrawing();
            BeginMode3D(*cameraController.camera);
                ClearBackground(DARKBLUE);
                DrawGrid(200, 1.0f);
                for (GameObject* gameObject : gameObjects)
                {
                    gameObject->Render();
                }
            EndMode3D();
        EndDrawing();
        break;
    }
}

void Game::Update(float lastLoopTime)
{
    if(GetTime() - lastLoopTime >= gameConfig->updateTime);
}

void Game::HandleKeyEvents()
{
    if(IsKeyPressed(KEY_R))
    {
        Utilities::Log("R pressed.", "GAME");
    }
}

int Game::Run()
{

    if (!m_Initialized)
    {
        Utilities::Log("Game is not initialized. Exiting...", "GAME", LOG_ERROR);
        return 1;
    }
    
    unsigned int returnCode = 0;
    float lastLoopTime = GetTime();
    
    gameState = GameState::Playing;
    while (!WindowShouldClose())  // Main loop.
    {
        HandleKeyEvents();
        Update(lastLoopTime);
        Render();
        lastLoopTime = GetTime();
    }

    // CloseWindow();
    return returnCode;
}