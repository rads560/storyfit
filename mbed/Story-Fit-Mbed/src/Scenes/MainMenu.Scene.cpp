#include "MainMenu.Scene.h"

#include "storyfit.h"
#include "textbox.h"
#include "rectangle.h"
#include "sensors.h"
#include "Title.Scene.h"

MainMenu::MainMenu(Game* game, Scene* parent)
    : Scene(game, parent) {
    // Reset button variables
    mLeftPressed[0] = false;
    mLeftPressed[1] = false;
    mRightPressed[0] = false;
    mRightPressed[1] = false;
    mSelectPressed[0] = false;
    mSelectPressed[1] = false;
}

MainMenu::~MainMenu() {
    Unload();
}

void MainMenu::Load() {
    //Main Menu Title text
    TextBox* tbTitle = new TextBox(this, 0, 5, 5, "StoryFit", 0, 0, 0xFFFF, 0x0000, 3);
    // Title text border
    Rectangle* titleBorder = new Rectangle(this, 1, -1, -1, 242, 10 + tbTitle->GetHeight(), Rectangle::RectType::FilledOutline);
    titleBorder->SetColor(0x0000);
    titleBorder->SetOutlineColor(0xFFFF);

    // Start Button
    mStartText = new TextBox(this, 2, 0, 99, "Quest!", 120, 20, HIGHLIGH_TEXT_COLOR, HIGHLIGHT_BG_COLOR, 2);
    mStartBorder = new Rectangle(this, 3, -1, 99, 120, 22, Rectangle::RectType::FilledOutline);
    mStartBorder->SetColor(HIGHLIGHT_BG_COLOR);
    mStartBorder->SetOutlineColor(HIGHLIGH_TEXT_COLOR);

    // Options Button
    mOptionsText = new TextBox(this, 2, 0, 120, "Options", 120, 20, BUTTON_TEXT_COLOR, BUTTON_BG_COLOR, 2);
    mOptionsBorder = new Rectangle(this, 3, -1, 120, 120, 22, Rectangle::RectType::FilledOutline);
    mOptionsBorder->SetColor(BUTTON_BG_COLOR);
    mOptionsBorder->SetOutlineColor(BUTTON_TEXT_COLOR);

    // Quit Button
    mQuitText = new TextBox(this, 2, 0, 141, "Quit", 120, 20, BUTTON_TEXT_COLOR, BUTTON_BG_COLOR, 2);
    mQuitBorder = new Rectangle(this, 3, -1, 141, 120, 22, Rectangle::RectType::FilledOutline);
    mQuitBorder->SetColor(BUTTON_BG_COLOR);
    mQuitBorder->SetOutlineColor(BUTTON_TEXT_COLOR);

    // Main Menu background
    Rectangle* bg = new Rectangle(this, 15, 0, 0, 240, 240);
    bg->SetColor(0x2945);
}

void MainMenu::Unload() {
}

void MainMenu::ProcessInput() {
    // Grab button input (Leading edge press only)
    mLeftPressed[1] = mLeftPressed[0];
    mRightPressed[1] = mRightPressed[0];
    mSelectPressed[1] = mSelectPressed[0];

    mLeftPressed[0] = Sensors::GetButtonLeft();
    mRightPressed[0] = Sensors::GetButtonRight();
    mSelectPressed[0] = Sensors::GetButtonCenter();
}

void MainMenu::UpdateSelection(int8_t next) {
    // Unhighlight the current selection
    TextBox* selText = nullptr;
    Rectangle* selBord = nullptr;
    switch(mSelected) {
        case MenuSelection::Start:
            selText = mStartText;
            selBord = mStartBorder;
            break;
        case MenuSelection::Options:
            selText = mOptionsText;
            selBord = mOptionsBorder;
            break;
        case MenuSelection::Quit:
            selText = mQuitText;
            selBord = mQuitBorder;
            break;
    }
    selText->SetColor(BUTTON_TEXT_COLOR);
    selText->SetBackground(BUTTON_BG_COLOR);
    selBord->SetColor(BUTTON_BG_COLOR);
    selBord->SetOutlineColor(BUTTON_TEXT_COLOR);

    // Highlight the new selection
    mSelected = next;
    switch(mSelected) {
        case MenuSelection::Start:
            selText = mStartText;
            selBord = mStartBorder;
            break;
        case MenuSelection::Options:
            selText = mOptionsText;
            selBord = mOptionsBorder;
            break;
        case MenuSelection::Quit:
            selText = mQuitText;
            selBord = mQuitBorder;
            break;
    }
    selText->SetColor(HIGHLIGH_TEXT_COLOR);
    selText->SetBackground(HIGHLIGHT_BG_COLOR);
    selBord->SetColor(HIGHLIGHT_BG_COLOR);
    selBord->SetOutlineColor(HIGHLIGH_TEXT_COLOR);
}

void MainMenu::Update(float deltaTime) {
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
            case MenuSelection::Start:
                break;
            case MenuSelection::Options:
                break;
            case MenuSelection::Quit:
                // Go back to the title screen
                mGame->LoadScene(new TitleScene(mGame));
                break;
        }
    }
}