#include "raylib.h"
#include "player.hpp"
#include "camera.hpp"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Mr Angry Cube - C++");
    SetTargetFPS(60);

    {
        MrAngryCube player({ 0.0f, 1.0f, 0.0f });
        FollowCamera followCamera(&player, { 10.0f, 10.0f, 10.0f });

        while (!WindowShouldClose()) {
            // Input
            if (IsKeyPressed(KEY_W)) player.behaviour.nextHeading = Heading::EAST;
            else if (IsKeyPressed(KEY_S)) player.behaviour.nextHeading = Heading::WEST;
            else if (IsKeyPressed(KEY_D)) player.behaviour.nextHeading = Heading::NORTH;
            else if (IsKeyPressed(KEY_A)) player.behaviour.nextHeading = Heading::SOUTH;
            else if (IsKeyPressed(KEY_Q)) player.behaviour.nextHeading = Heading::NO_HEADING;
            else if (IsKeyPressed(KEY_SPACE)) player.behaviour.canMove = true;

            // Update
            player.Update();
            followCamera.Update();

            // Draw
            BeginDrawing();
            ClearBackground(DARKBLUE);

            BeginMode3D(followCamera.GetCamera());
            DrawGrid(20, 1.0f);
            player.Draw();
            EndMode3D();

            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
