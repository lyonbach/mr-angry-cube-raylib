
#include "raylib.h"
#include "raymath.h"
#define GLSL_VERSION 330

#include <filesystem>


Vector2 VecSin(Vector2 vec) {
    return (Vector2){sin(DEG2RAD * (45 + abs((int)vec.x) % 90)), sin(DEG2RAD * (45 + abs((int)vec.y % 90)))};
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    std::filesystem::path fs = std::filesystem::path(__FILE__).parent_path();
    std::string texturePathStr = (fs / "textures" / "concrete.png").string();
    std::string shaderPathStr = (fs / "vendor/raylib/examples/shaders/resources/shaders/glsl330" / "blur.fs").string();

    const int screenWidth = 800;
    const int screenHeight = 450;
    const float cubeSize = 4.0f;
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
    // camera.projection = CAMERA_ORTHOGRAPHIC;             // Camera mode type FIXME REMOVE

    int quarterRotation = 90;
    int increment = 1;

    Vector3 cubePosition = { 0.0f, cubeSize/2, 0.0f };
    Vector3 rotateAxis = { 0.0f, 0.0f, 1.0f };
    Vector3 cubeRotation = { 0.0f, 0.0f, 0.0f };
    Vector3 nextRotateAxis = rotateAxis;

    Model model = LoadModel("/media/lyonbach/depot/Work/Projects/codeoflyonbach/MrAngryCubeTest/models/mr_angry_cube.obj");
    Texture texture = LoadTexture(texturePathStr.c_str());
    Shader shader = LoadShader(0, shaderPathStr.c_str());
    Material material = LoadMaterialDefault();
    
    material.shader = shader;
    SetMaterialTexture(&material, MATERIAL_MAP_DIFFUSE, texture);
    
    Mesh mrAngryCubeMesh = model.meshes[0];

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Define Variables
    Matrix transform = MatrixIdentity();

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
        } else if (IsKeyPressed(KEY_Q))
        {
            nextRotateAxis = { 0.0f, -1.0f, 0.0f };
        } else if (IsKeyPressed(KEY_E))
        {
            nextRotateAxis = { 0.0f, 1.0f, 0.0f };
        }
        //----------------------------------------------------------------------------------
        
        // Update
        //----------------------------------------------------------------------------------
        
        // Update cube rotation. We basically calculate the cube vertical displacement and
        // update a 2d vector. We first divide the vector to half cube size then can multiply
        // the x and y values of the vector to update the cube vertical position. 
        cubeRotation = Vector3Add(cubeRotation, Vector3Scale(rotateAxis, increment));

        // Calculate the vertical position of the cube, x stands for the rotation around x axis
        // and y stands for the rotation around z axis. We do not need the other axis, 
        // we can use 2d vector to calculate the vertical position of the cube.
        Vector2 deltaY = Vector2Scale(Vector2Scale(VecSin((Vector2){cubeRotation.x, cubeRotation.z}), cubeHypotenuse), 1/cubeHalfSize);

        
        // transform = MatrixMultiply(MatrixTranslate(0.0f, 0.0f, 0.0f), transform);
        transform.m12 = -cubeRotation.z / 180.0f * cubeSize;
        transform.m13 = deltaY.y * deltaY.x;
        transform.m14 = cubeRotation.x / 180.0f * cubeSize;

        // We need to update the cube position via offsetting. So we need to update the transform matrix.
        transform = MatrixMultiply(transform, MatrixRotate(rotateAxis, DEG2RAD * increment));

        TraceLog(LOG_INFO, "rotation: %f %f %f", cubeRotation.x, cubeRotation.y, cubeRotation.z);
        TraceLog(LOG_INFO, "height: %f %f", deltaY.x, deltaY.y);


        // Set next rotation axis when cube is exactly at 90° increments.
        if ((int)cubeRotation.x % quarterRotation == 0 && rotateAxis.x != 0.0f || 
            (int)cubeRotation.z % quarterRotation == 0 && rotateAxis.z != 0.0f ||
            (int)cubeRotation.y % quarterRotation == 0 && rotateAxis.y != 0.0f)
        {
            rotateAxis = nextRotateAxis;
        }


        camera.target = (Vector3){transform.m12, transform.m13, transform.m14};
        camera.position = (Vector3){camera.target.x, camera.target.y + 5, camera.target.z - 10.0f};
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(DARKBLUE);
            BeginMode3D(camera);
                DrawMesh(mrAngryCubeMesh, material, transform);
                DrawGrid(10, 1.0f);
            EndMode3D();
            DrawFPS(10, 10);
            DrawText("Test Mr. Angry Cube", 10, 30, 20, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(shader);       // Unload shader
    // UnloadTexture(texture);     // Unload texture
    UnloadModel(model);         // Unload model
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
