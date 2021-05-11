/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include "Title.Scene.h"

#include "storyfit.h"
#include "textbox.h"
#include "rectangle.h"
#include "sensors.h"
#include "MainMenu.Scene.h"

TitleScene::TitleScene(Game* game, Scene* parent)
    : Scene(game, parent) {
}

TitleScene::~TitleScene() {
    Unload();
}

void TitleScene::Load() {
    // Serial.begin(9600);
    // Serial.println("StoryFit Title Screen");

    //Title screen Big text
    tbTitle = new TextBox(this, 1, "Title", 30, 80, "StoryFit", 0, 0, 0x4E59, 0x4970, 3);
    // Title screen prompt
    tbPrompt = new TextBox(this, 0, "Prompt", 70, 190, "Press Start", 0, 0, 0xFFFF, 0x4970, 1);
    // Title screen background
    Rectangle* bg = new Rectangle(this, 15, "Background", 0, 0, 240, 240);
    bg->SetColor(0x4970);
    // Setup flashing
    mTimer = PROMPT_FLASH_PERIOD;
    mPromptHighlight = false;
    // Serial.println("Finished Initializing");
}

void TitleScene::Unload() {
    if (tbTitle != nullptr) {
        delete tbTitle;
        tbTitle = nullptr;
    }
    if (tbPrompt != nullptr) {
        delete tbPrompt;
        tbPrompt = nullptr;
    }
}

void TitleScene::ProcessInput() {
    mStartPressed = Sensors::GetButtonCenter();
}

void TitleScene::Update(float deltaTime) {
    mTimer -= deltaTime;
    if (mTimer <= 0.0f) {
        if (mPromptHighlight) {
            tbPrompt->SetColor(0xFFFF);
        }
        else {
            tbPrompt->SetColor(0xF72B);
        }
        mPromptHighlight = !mPromptHighlight;
        mTimer = PROMPT_FLASH_PERIOD;
    }

    // If the start button was pressed, move to the Main Menu scene
    if (mStartPressed) {
        // Load Scene automatically unloads the current scene
        mGame->LoadScene(new MainMenu(mGame));
    }
}