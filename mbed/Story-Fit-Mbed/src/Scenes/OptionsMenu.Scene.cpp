/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include "OptionsMenu.Scene.h"

#include "storyfit.h"
#include "textbox.h"
#include "rectangle.h"
#include "sensors.h"
#include "HRMTest.Scene.h"
#include "BMETest.Scene.h"
#include "MainMenu.Scene.h"

OptionsMenu::OptionsMenu(Game* game, Scene* parent)
    : Scene(game, parent) {
    // Reset button variables
    mLeftPressed[0] = false;
    mLeftPressed[1] = false;
    mRightPressed[0] = false;
    mRightPressed[1] = false;
    mSelectPressed[0] = false;
    mSelectPressed[1] = false;
}

OptionsMenu::~OptionsMenu() {
    Unload();
}

void OptionsMenu::Load() {
    //Main Menu Title text
    TextBox* tbTitle = new TextBox(this, 0, "Options Title", 5, 5, "Options", 0, 0, 0xFFFF, 0x0000, 3);
    // Title text border
    Rectangle* titleBorder = new Rectangle(this, 1, "Options Title Border", -1, -1, 242, 10 + tbTitle->GetHeight(), Rectangle::RectType::FilledOutline, 0x0000, 0xFFFF);

    // HRM Button
    mHRMText = new TextBox(this, 2, "HRM Test Button", 0, 98, "HRM Test", MENU_WIDTH, 20, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, 2);
    mHRMBorder = new Rectangle(this, 3, "HRM Test Border", -1, mHRMText->GetY() - 1, MENU_WIDTH, mHRMText->GetHeight() + 2, Rectangle::RectType::FilledOutline, HIGHLIGHT_BG_COLOR, HIGHLIGHT_TEXT_COLOR);
    mHRMBorder->SetColor(HIGHLIGHT_BG_COLOR);
    mHRMBorder->SetOutlineColor(HIGHLIGHT_TEXT_COLOR);

    // Options Button
    mBMEText = new TextBox(this, 2, "BME Test Button", 0, mHRMText->GetY() + mHRMText->GetHeight() + 2, "BME Test", MENU_WIDTH, 20, BUTTON_TEXT_COLOR, BUTTON_BG_COLOR, 2);
    mBMEBorder = new Rectangle(this, 3, "BME Test Border", -1, mBMEText->GetY() - 1, MENU_WIDTH, mBMEText->GetHeight() + 2, Rectangle::RectType::FilledOutline, BUTTON_BG_COLOR, BUTTON_TEXT_COLOR);

    // Back Button
    mBackText = new TextBox(this, 2, "Back Button", 0, mBMEText->GetY() + mBMEText->GetHeight() + 2, "Back", MENU_WIDTH, 20, BUTTON_TEXT_COLOR, BUTTON_BG_COLOR, 2);
    mBackBorder = new Rectangle(this, 3, "Back Border", -1, mBackText->GetY() - 1, MENU_WIDTH, mBackText->GetHeight() + 2, Rectangle::RectType::FilledOutline, BUTTON_BG_COLOR, BUTTON_TEXT_COLOR);

    // Main Menu background
    Rectangle* bg = new Rectangle(this, 15, "Menu Background", 0, 0, 240, 240, Rectangle::RectType::Filled, 0x2945);
}

void OptionsMenu::Unload() {
}

void OptionsMenu::ProcessInput() {
    // Grab button input (Leading edge press only)
    mLeftPressed[1] = mLeftPressed[0];
    mRightPressed[1] = mRightPressed[0];
    mSelectPressed[1] = mSelectPressed[0];

    mLeftPressed[0] = Sensors::GetButtonLeft();
    mRightPressed[0] = Sensors::GetButtonRight();
    mSelectPressed[0] = Sensors::GetButtonCenter();
}

void OptionsMenu::UpdateSelection(int8_t next) {
    // Unhighlight the current selection
    TextBox* selText = nullptr;
    Rectangle* selBord = nullptr;
    switch(mSelected) {
        case MenuSelection::HRM:
            selText = mHRMText;
            selBord = mHRMBorder;
            break;
        case MenuSelection::BME:
            selText = mBMEText;
            selBord = mBMEBorder;
            break;
        case MenuSelection::Back:
            selText = mBackText;
            selBord = mBackBorder;
            break;
    }
    selText->SetColor(BUTTON_TEXT_COLOR);
    selText->SetBackground(BUTTON_BG_COLOR);
    selBord->SetColor(BUTTON_BG_COLOR);
    selBord->SetOutlineColor(BUTTON_TEXT_COLOR);

    // Highlight the new selection
    mSelected = next;
    switch(mSelected) {
        case MenuSelection::HRM:
            selText = mHRMText;
            selBord = mHRMBorder;
            break;
        case MenuSelection::BME:
            selText = mBMEText;
            selBord = mBMEBorder;
            break;
        case MenuSelection::Back:
            selText = mBackText;
            selBord = mBackBorder;
            break;
    }
    selText->SetColor(HIGHLIGHT_TEXT_COLOR);
    selText->SetBackground(HIGHLIGHT_BG_COLOR);
    selBord->SetColor(HIGHLIGHT_BG_COLOR);
    selBord->SetOutlineColor(HIGHLIGHT_TEXT_COLOR);
}

void OptionsMenu::Update(float deltaTime) {
    // Check for leading edge left button press
    if (mLeftPressed[0] && !mLeftPressed[1]) {
        // Serial.println("Left Pressed");
        // Move menu selection up
        int8_t next = mSelected - 1;
        if (next < 0) {
            next = NUM_MENU_OPTIONS - 1;
        }
        // Update the selection
        UpdateSelection(next);
    }
    // Check for leading edge right button press
    if (mRightPressed[0] && !mRightPressed[1]) {
        // Serial.println("Right Pressed");
        // Move menu selection down
        int8_t next = mSelected + 1;
        if (next >= NUM_MENU_OPTIONS) {
            next = 0;
        }
        // Update the selection
        UpdateSelection(next);
    }
    // Check for leading edge select button press
    if (mSelectPressed[0] && !mSelectPressed[1]) {
        // Go the next scene based on the selection
        switch(mSelected) {
            case MenuSelection::HRM:
                // Go to HRM Test
                mGame->LoadScene(new HRMTest(mGame));
                break;
            case MenuSelection::BME:
                // Go to BME Test
                mGame->LoadScene(new BMETest(mGame));
                break;
            case MenuSelection::Back:
                // Go back to the title screen
                mGame->LoadScene(new MainMenu(mGame));
                break;
        }
    }
}