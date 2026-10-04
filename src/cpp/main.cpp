#include "raylib.h"
#include "game.hpp"
#include "player.hpp"
#include "camera.hpp"

int main() {
    Game& game = Game::getInstance();
    game.LoadSettings("build/settings.json");

    if (game.settings.empty()) {
        std::cerr << "Critical Error: Settings not loaded. Using defaults." << std::endl;
        // Fallback logic or exit
    }

    if (game.settings["window"].contains("full_screen") && game.settings["window"]["full_screen"].get<bool>()) {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }


    const int screenWidth = game.settings["window"]["width"];
    const int screenHeight = game.settings["window"]["height"];
    const char* title = game.settings["window"]["title"].get<std::string>().c_str();

    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(game.settings["window"]["fps"]);

    {
        Vector3 startPos = { 
            game.settings["player"]["start_position"][0], 
            game.settings["player"]["start_position"][1], 
            game.settings["player"]["start_position"][2] 
        };
        MrAngryCube player(startPos);
        
        Vector3 camOffset = { 
            game.settings["camera"]["offset"][0], 
            game.settings["camera"]["offset"][1], 
            game.settings["camera"]["offset"][2] 
        };
        float fovy = game.settings["camera"]["fovy"];
        FollowCamera followCamera(&player, camOffset, fovy);

        while (!WindowShouldClose()) {
            // Input
            if (IsKeyPressed(KEY_W)) player.behaviour.nextHeading = Heading::NORTH;
            else if (IsKeyPressed(KEY_S)) player.behaviour.nextHeading = Heading::SOUTH;
            else if (IsKeyPressed(KEY_D)) player.behaviour.nextHeading = Heading::EAST;
            else if (IsKeyPressed(KEY_A)) player.behaviour.nextHeading = Heading::WEST;
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
