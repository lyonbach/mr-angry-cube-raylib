#include "GameObject.h" 

#include "raylib.h"
#include "raymath.h"
#define GLSL_VERSION 330

#include <filesystem>


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float cubeSize = 1.0f;
    const float cubeHalfSize = cubeSize / 2.0f;
    const float cubeHypotenuse = sqrt(cubeHalfSize * cubeHalfSize * 2);

    InitWindow(screenWidth, screenHeight, "Mr Angry Cube Test - V 0.1");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, -5.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    int quarterRotation = 90;
    float speed = 5.0f;

    Vector3 rotateAxis = { 0.0f, 0.0f, 1.0f };
    Vector3 nextRotateAxis = rotateAxis;
    
    GameObject *mrAngryCube = new GameObject();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Define Variables
    Matrix transform = MatrixScale(cubeSize, cubeSize, cubeSize);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Handle keypress
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_W))
        {
            nextRotateAxis = { 1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_S))
        {
            nextRotateAxis = { -1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_A))
        {
            nextRotateAxis = { 0.0f, 0.0f, -1.0f };
        } else if (IsKeyPressed(KEY_D))
        {
            nextRotateAxis = { 0.0f, 0.0f, 1.0f };
        } else if (IsKeyPressed(KEY_E))
        {
            nextRotateAxis = { 0.0f, -1.0f, 0.0f };
        } else if (IsKeyPressed(KEY_Q))
        {
            nextRotateAxis = { 0.0f, 1.0f, 0.0f };
        }
        //----------------------------------------------------------------------------------
        
        // Update
        //----------------------------------------------------------------------------------
        mrAngryCube->Update(rotateAxis, speed);

        Vector3* rotation = &mrAngryCube->m_Rotation;
        
        if ((int)rotation->x % quarterRotation == 0 && rotateAxis.x != 0.0f || 
            (int)rotation->z % quarterRotation == 0 && rotateAxis.z != 0.0f ||
            (int)rotation->y % quarterRotation == 0 && rotateAxis.y != 0.0f ||
            (rotateAxis.x == 0.0f && rotateAxis.z == 0.0f && rotateAxis.y == 0.0f))
        {
                rotateAxis = nextRotateAxis;
        }
            
        camera.target = (Vector3){mrAngryCube->m_Transform.m12, mrAngryCube->m_Transform.m13, mrAngryCube->m_Transform.m14};
        camera.position = (Vector3){camera.target.x, camera.target.y + 5, camera.target.z - 10.0f};
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(DARKBLUE);
            BeginMode3D(camera);
                mrAngryCube->Render();
                DrawGrid(10, 1.0f);
            EndMode3D();
            DrawFPS(10, 10);
            DrawText("Test Mr. Angry Cube", 10, 30, 20, WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    delete mrAngryCube;
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
