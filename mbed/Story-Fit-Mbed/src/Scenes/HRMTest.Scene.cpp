#include "HRMTest.Scene.h"

#include "storyfit.h"
#include "textbox.h"
#include "rectangle.h"
#include "sensors.h"
#include "OptionsMenu.Scene.h"

HRMTest::HRMTest(Game* game, Scene* parent)
    : Scene(game, parent) {
}
HRMTest::~HRMTest() {
    Unload();
}

void HRMTest::Load() {
    //Title text
    TextBox* tbTitle = new TextBox(this, 0, "HRM Test Title", 5, 5, "Heartrate Test", 0, 0, 0xFFFF, 0x0000, 2);
    // Title text border
    Rectangle* titleBorder = new Rectangle(this, 1, "HRM Test Title Border", -1, -1, 242, 10 + tbTitle->GetHeight(), Rectangle::RectType::FilledOutline, 0x0000, 0xFFFF);

    // Big, centered number
    mBPMValueText = new TextBox(this, 1, "BPM Value", 0, 0, "?", 0, 0, 0xFFFF, 0x0000, 3);
    // Apply centering
    mBPMValueText->SetX(120 - (mBPMValueText->GetWidth() / 2));
    mBPMValueText->SetY(120 - (tbTitle->GetHeight() / 2) - (mBPMValueText->GetHeight() / 2));

    // Background for the value, to erase old value
    mBPMValueBG = new Rectangle(this, 2, "BPM Value Background", 0, mBPMValueText->GetY(), 98, mBPMValueText->GetHeight(), Rectangle::RectType::Filled, 0x0000);
    // Apply centering for x
    mBPMValueBG->SetX(120 - (mBPMValueBG->GetWidth() / 2));

    // "BPM" text positioned centered, under the value text
    mBPMText = new TextBox(this, 1, "BPM Text", 0, mBPMValueText->GetY() + mBPMValueText->GetHeight() + 5, "BPM", 0, 0, 0xFFFF, 0x0000, 2);
    // Apply centering for x
    mBPMText->SetX(120 - (mBPMText->GetWidth() / 2));

    // Border for BPM value and text
    mBPMBorder = new Rectangle(this, 2, "BPM Border", 0, mBPMValueText->GetY() - 5, 100, mBPMValueText->GetHeight() + mBPMText->GetHeight() + 15, Rectangle::RectType::Outline, 0x0000, 0xFFFF);
    // Apply centering for x
    mBPMBorder->SetX(120 - (mBPMBorder->GetWidth() / 2));

    // "Sensor Not Connected" text, displayed when the sensor is not connected
    mBPMStatus = new TextBox(this, 1, "BPM Status", 0, mBPMBorder->GetY() + mBPMBorder->GetHeight() + 5, "HRM Disconnected", 0, 0, 0xF800, 0x0000, 1);
    // Apply centering for x
    mBPMStatus->SetX(120 - (mBPMStatus->GetWidth() / 2));
    // Disable (for now)
    mBPMStatus->SetIsActive(false);

    // Background
    Rectangle* bg = new Rectangle(this, 15, "HRM Test Background", 0, 0, 240, 240, Rectangle::RectType::Filled, 0x0000);
}

void HRMTest::Unload() {

}

void HRMTest::ProcessInput() {
    mBPM[1] = mBPM[0];
    mConnected = Sensors::GetHeart(mBPM[0]);
    mBackPressed[1] = mBackPressed[0];
    mBackPressed[0] = Sensors::GetButtonCenter();
}

void HRMTest::Update(float deltaTime) {
    // Update the BPM value text
    if (mBPM[0] != mBPM[1]) {
        char vstr[10];
        itoa(mBPM[0], vstr, 10);
        mBPMValueText->SetText(vstr);
        mBPMValueText->UpdateCanvas();
        // Reapply x centering
        mBPMValueText->SetX(120 - (mBPMValueText->GetWidth() / 2));
        // Reapply the background
        mBPMValueBG->SetIsActive(true);
    }
    // If the sensor is disconnected, activate the status text
    mBPMStatus->SetIsActive(!mConnected);

    // Check for leading edge press on back button
    if (mBackPressed[0] && mBackPressed[0] != mBackPressed[1]) {
        // Go back to the options menu
        mGame->LoadScene(new OptionsMenu(mGame));
    }
}