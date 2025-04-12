#include "GameObject.h"
#include "Game.h"
#include "MrAngryCube.h"
#include "Enemy.h"

#include "raylib.h"
#include "raymath.h"
#define GLSL_VERSION 330

#include <filesystem>
#include <vector>


std::filesystem::path fs = std::filesystem::path(__FILE__).parent_path();
std::string texturePath = (fs / "../textures" / "texel_checker.png").string();
std::string shaderPath = (fs / "../vendor/raylib/examples/shaders/resources/shaders/glsl330" / "blur.fs").string();
std::string modelPath = (fs / "../models" / "mr_angry_cube.obj").string();

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Mr Angry Cube Test - V 0.1");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, -5.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    int quarterRotation = 90;

    // Instantiate the game and game specifications.
    Game* game = new Game();
    game->m_UpdateSpeed = 100;
    

    MrAngryCube *mrAngryCube = new MrAngryCube(texturePath.c_str(), shaderPath.c_str(), modelPath.c_str());
    mrAngryCube->m_Speed = 2.0f;
    game->Register(mrAngryCube);
    game->Register(new Enemy(texturePath.c_str(), shaderPath.c_str(), modelPath.c_str()));

    SetTargetFPS(240);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Handle keypress
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
        }
        //----------------------------------------------------------------------------------

        // Update
        //----------------------------------------------------------------------------------
        game->Update();
        // FIXME move to a function
        if ((int)mrAngryCube->m_Rotation.x % quarterRotation == 0 && mrAngryCube->m_RotationAxis.x != 0.0f ||
            (int)mrAngryCube->m_Rotation.z % quarterRotation == 0 && mrAngryCube->m_RotationAxis.z != 0.0f ||
            (int)mrAngryCube->m_Rotation.y % quarterRotation == 0 && mrAngryCube->m_RotationAxis.y != 0.0f ||
            (mrAngryCube->m_RotationAxis.x == 0.0f &&
             mrAngryCube->m_RotationAxis.z == 0.0f &&
             mrAngryCube->m_RotationAxis.y == 0.0f))
        {
            mrAngryCube->m_RotationAxis = mrAngryCube->m_NextRotationAxis;
            if (mrAngryCube->IsFaceOnTheGround())
            {
                for (Enemy* enemy : game->GetCollidingEnemies())
                {
                    TraceLog(LOG_WARNING, "Collides!");
                }
            }
        }

        camera.target = (Vector3){mrAngryCube->m_Transform.m12, mrAngryCube->m_Transform.m13, mrAngryCube->m_Transform.m14};
        camera.position = (Vector3){camera.target.x, camera.target.y + 5, camera.target.z - 20.0f};
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(DARKBLUE);
            BeginMode3D(camera);
                game->Render();
                DrawGrid(10, 1.0f);
            EndMode3D();
            DrawFPS(10, 10);
            DrawText(("Target Update Rate: " + std::to_string(game->m_UpdateSpeed)).c_str(), 10, 30, 20, WHITE);
            DrawText(("DeltaTime: " + std::to_string(std::round((GetTime() - game->m_LastUpdateTime) * 1000) / 1000)).c_str(), 10, 50, 20, WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    delete game;
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
