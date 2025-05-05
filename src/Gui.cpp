#define RAYGUI_IMPLEMENTATION
#include "Gui.h"
#include "Constants.h"

std::map<std::string, bool> Menu::GetButtonStates()
{
    return buttonStates;
}

void Menu::Render()
{
    int i = 0;
    for (auto& pair : buttonStates)
    {
        buttonStates[pair.first] = GuiButton({
            (float)GetScreenWidth() / 8,
            GetScreenHeight() - (rectangle.y + 70 * i),
            (float)buttonWidth, (float)buttonHeight},
            pair.first.c_str()
           );
        ++i;
    }
}

void Menu::Update()
{
    for (auto& pair : buttonStates)
    {
        buttonStates[pair.first] = false;
    }
}

MainMenu::MainMenu()
{
    buttonStates = {
        { NEW_GAME_BUTTON_TEXT, false },
        { LOAD_LEVEL_BUTTON_TEXT, false },
        { EXIT_GAME_BUTTON_TEXT, false },
    };
}

PauseMenu::PauseMenu()
{
    buttonStates = {
        { CONTINUE_BUTTON_TEXT, false },
        { EXIT_GAME_BUTTON_TEXT, false },
    };
}


void Gui::Init()
{
    GuiLoadStyle("gui/style_rltech.rgs");
}