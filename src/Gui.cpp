#define RAYGUI_IMPLEMENTATION
#include "Gui.h"
#include "Constants.h"
#include <algorithm>


std::map<std::string, bool> Menu::GetButtonStates()
{
    return buttonStates;
}


Menu::Menu(Texture& background)
    : m_BackgroundTexture(background)
{}

void Menu::Render()
{

    // Calculate offset and scale
    float imageWidth = m_BackgroundTexture.width;
    float imageHeight = m_BackgroundTexture.height;
    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();
    float scale = screenWidth >= screenHeight ? screenWidth / imageWidth : screenHeight / imageHeight;

    DrawTextureEx(m_BackgroundTexture, {0, 0}, 0, scale, WHITE);
    int i = 0;
    for (auto& pair : buttonStates)
    {
        buttonStates[pair.first] = GuiButton({
            rectangle.x,
            GetScreenHeight() - (rectangle.y + (buttonHeight + offsetY) * i),
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

MainMenu::MainMenu(Texture& background)
    : Menu(background)
{
    buttonStates = {
        { NEW_GAME_BUTTON_TEXT, false },
        { LOAD_LEVEL_BUTTON_TEXT, false },
        { EXIT_GAME_BUTTON_TEXT, false },
    };
}

PauseMenu::PauseMenu(Texture& background)
    : Menu(background)
{
    buttonStates = {
        { CONTINUE_BUTTON_TEXT, false },
        { RETURN_TO_MAIN_MENU_BUTTON_TEXT, false },
        { EXIT_GAME_BUTTON_TEXT, false },
    };
}

LevelMenu::LevelMenu(Texture& background)
    : Menu(background)
{
    buttonStates = {
        { SELECT_LEVEL_BUTTON_TEXT, false },
        { RETURN_TO_MAIN_MENU_BUTTON_TEXT, false }
    };

    for (const auto& entry : std::filesystem::directory_iterator("./assets/levels"))
    {
        if (entry.is_regular_file())
        {
            levels.push_back(entry.path().filename().stem().string());
        }
    }
    std::sort(levels.begin(), levels.end());

    levelsText = "";
    for (size_t i = 0; i < levels.size(); ++i)
    {
        levelsText += levels[i];
        if (i < levels.size() - 1)
        {
            levelsText += ";";
        }
    }

}

void LevelMenu::Render()
{
    Menu::Render();
    clicked = GuiComboBox({menuX, comboBoxY, buttonWidth, buttonHeight}, levelsText.c_str(), &selected);
    if(clicked)
    {
        selected++;
        selected = selected % levels.size();
    };
}


void Gui::Init()
{
    GuiLoadStyle("gui/style_rltech.rgs");
}