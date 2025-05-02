#define RAYGUI_IMPLEMENTATION
#include "Gui.h"

MainMenu::MainMenu()
{
    buttonStates = {
        {"new_game", false},
        {"load_level", false},
        {"exit_game", false}
    };
}

void MainMenu::RenderAndUpdate()
{
    buttonStates["new_game"] = GuiButton(
        {(float)GetScreenWidth() / 6,
         rectangle.y + 30, (float)buttonWidth,
        (float)buttonHeight},
        "New Game"
    );
    buttonStates["load_game"] = GuiButton(
        {(float)GetScreenWidth() / 6,
         rectangle.y + 30 + 10 + buttonHeight,
        (float)buttonWidth, (float)buttonHeight},
        "Load Level"
    );
    buttonStates["exit_game"] = GuiButton(
        {(float)GetScreenWidth() / 6,
         rectangle.y + 30 + 2*(10 + buttonHeight),
        (float)buttonWidth, (float)buttonHeight},
        "Exit Game"
    );
}

std::map<std::string, bool> MainMenu::GetButtonStates()
{
    return buttonStates;
}

void Gui::Init()
{
    GuiLoadStyle("gui/style_rltech.rgs");
}