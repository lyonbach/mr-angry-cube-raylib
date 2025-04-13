#pragma once

#include "Game.h"
#include "raylib.h"
#include <string>
#include <functional>

typedef void(*ButtonCallback)();

class GuiItem {
    public:
        Game* m_Game;
        Vector2 m_Position;
        virtual void Draw() = 0;
        virtual void Update() = 0;
};


class PushButton : public GuiItem {
    public:
        Game* m_Game;
        std::string m_Text;
        Vector2 m_Position;

        PushButton(Game* game, const std::string& text, int x, int y, ButtonCallback callbackFunction=nullptr);
        void Draw() override;
        void Update() override;
        void DoAction();
        
        private:
        ButtonCallback callback;
        int fontSize;
        int paddingX;
        int paddingY;
        int textWidth;
        int startX;
        int startY;
        int endX;
        int endY;
        bool shouldUpdate;
        Color currentBackgroundColor;
};
