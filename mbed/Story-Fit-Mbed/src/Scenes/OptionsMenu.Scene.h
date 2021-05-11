/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include "scene.h"

class OptionsMenu : public Scene {
    public:
        OptionsMenu(class Game* game, Scene* parent = nullptr);
        ~OptionsMenu();
        void Load();
        void Unload();

        void ProcessInput() override;
        void Update(float deltaTime) override;
    private:
        enum MenuSelection {
            HRM = 0,
            BME = 1,
            Back = 2
        };
        static constexpr int8_t NUM_MENU_OPTIONS = 3;

        class TextBox* mHRMText = nullptr;
        class Rectangle* mHRMBorder = nullptr;
        class TextBox* mBMEText = nullptr;
        class Rectangle* mBMEBorder = nullptr;
        class TextBox* mBackText = nullptr;
        class Rectangle* mBackBorder = nullptr;

        int8_t mSelected = MenuSelection::HRM;

        bool mLeftPressed[2];
        bool mRightPressed[2];
        bool mSelectPressed[2];

        void UpdateSelection(int8_t next);

        static constexpr uint16_t BUTTON_TEXT_COLOR = 0xFE05;
        static constexpr uint16_t BUTTON_BG_COLOR = 0x0014;
        static constexpr uint16_t HIGHLIGHT_TEXT_COLOR = 0x4E59;
        static constexpr uint16_t HIGHLIGHT_BG_COLOR = 0x4970;
        static constexpr uint16_t MENU_WIDTH = 240;
};