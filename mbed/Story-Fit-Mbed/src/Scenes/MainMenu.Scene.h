/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include "scene.h"

class MainMenu : public Scene {
    public:
        MainMenu(class Game* game, Scene* parent = nullptr);
        ~MainMenu();
        void Load();
        void Unload();

        void ProcessInput() override;
        void Update(float deltaTime) override;
    private:
        enum MenuSelection {
            Start = 0,
            Options = 1,
            Quit = 2
        };
        static constexpr int8_t NUM_MENU_OPTIONS = 3;

        class TextBox* mStartText = nullptr;
        class Rectangle* mStartBorder = nullptr;
        class TextBox* mOptionsText = nullptr;
        class Rectangle* mOptionsBorder = nullptr;
        class TextBox* mQuitText = nullptr;
        class Rectangle* mQuitBorder = nullptr;

        int8_t mSelected = MenuSelection::Start;

        bool mLeftPressed[2];
        bool mRightPressed[2];
        bool mSelectPressed[2];

        void UpdateSelection(int8_t next);

        static constexpr uint16_t BUTTON_TEXT_COLOR = 0xFE05;
        static constexpr uint16_t BUTTON_BG_COLOR = 0x0014;
        static constexpr uint16_t HIGHLIGHT_TEXT_COLOR = 0x4E59;
        static constexpr uint16_t HIGHLIGHT_BG_COLOR = 0x4970;
};