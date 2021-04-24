#include "Title.Scene.h"

#include "textbox.h"
#include "rectangle.h"

TitleScene::TitleScene(Game* game, Scene* parent)
    : Scene(game, parent) {
}

TitleScene::~TitleScene() {

}

void TitleScene::Load() {
    // Serial.begin(9600);
    // Serial.println("StoryFit Title Screen");
    // Title screen background
    Rectangle* bg = new Rectangle(this, 2, 0, 0, 240, 240);
    bg->SetColor(0x4970);

    //Title screen Big text
    tbTitle = new TextBox(this, 1, 30, 80, 180, 90, "StoryFit");
    tbTitle->SetColor(0x4E59);
    tbTitle->SetBackground(0x4970);
    tbTitle->SetSize(3);
    // Title screen prompt
    tbPrompt = new TextBox(this, 0, 70, 190, 100, 20, "Press Start");
    tbPrompt->SetColor(0xFFFF);
    tbPrompt->SetBackground(0x4970);
    tbPrompt->SetSize(1);
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

void TitleScene::Update(float deltaTime) {
    digitalWrite(LED_G, LOW);
    mTimer -= deltaTime;
    if (mTimer <= 0.0f) {
        if (mPromptHighlight) {
            digitalWrite(LED_R, HIGH);
            digitalWrite(LED_B, LOW);
            tbPrompt->SetColor(0xFFFF);
        }
        else {
            digitalWrite(LED_R, LOW);
            digitalWrite(LED_B, HIGH);
            tbPrompt->SetColor(0xF72B);
        }
        mPromptHighlight = !mPromptHighlight;
        mTimer = PROMPT_FLASH_PERIOD;
    }
}