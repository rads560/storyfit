/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include "BMETest.Scene.h"

#include "storyfit.h"
#include "textbox.h"
#include "rectangle.h"
#include "sensors.h"
#include "OptionsMenu.Scene.h"

BMETest::BMETest(Game* game, Scene* parent)
    : Scene(game, parent) {
}
BMETest::~BMETest() {
    Unload();
}

void BMETest::Load() {
    //Title text
    TextBox* tbTitle = new TextBox(this, 0, "BME Test Title", 5, 5, "BME Test", 0, 0, 0xFFFF, 0x0000, 2);
    // Title text border
    Rectangle* titleBorder = new Rectangle(this, 1, "BME Test Title Border", -1, -1, 242, 10 + tbTitle->GetHeight(), Rectangle::RectType::FilledOutline, 0x0000, 0xFFFF);

    // Big, centered number
    mTempValueText = new TextBox(this, 1, "Temp Value", 0, 0, "?", 0, 0, 0xFFFF, 0x0000, 2);
    // Apply centering
    mTempValueText->SetX(60 - (mTempValueText->GetWidth() / 2));
    mTempValueText->SetY(60 - (tbTitle->GetHeight() / 2) - (mTempValueText->GetHeight() / 2));
    mTempValueText->SetMaxHeight(mTempValueText->GetHeight());

    // Background for the value, to erase old value
    mTempValueBG = new Rectangle(this, 2, "Temp Value Background", 0, mTempValueText->GetY(), 98, mTempValueText->GetHeight(), Rectangle::RectType::Filled, 0x0000);
    // Apply centering for x
    mTempValueBG->SetX(60 - (mTempValueBG->GetWidth() / 2));

    // "Temp" text positioned centered, under the value text
    mTempText = new TextBox(this, 1, "Temp Text", 0, mTempValueText->GetY() + mTempValueText->GetHeight() + 5, "Temp (*C)", 0, 0, 0xFFFF, 0x0000, 1);
    // Apply centering for x
    mTempText->SetX(60 - (mTempText->GetWidth() / 2));

    // Border for Temp value and text
    mTempBorder = new Rectangle(this, 2, "Temp Border", 0, mTempValueText->GetY() - 5, 100, mTempValueText->GetHeight() + mTempText->GetHeight() + 15, Rectangle::RectType::Outline, 0x0000, 0xFFFF);
    // Apply centering for x
    mTempBorder->SetX(60 - (mTempBorder->GetWidth() / 2));

    // --------------------------

    // Big, centered number
    mHumidValueText = new TextBox(this, 1, "Humid Value", 0, 0, "?", 0, 0, 0xFFFF, 0x0000, 2);
    // Apply centering
    mHumidValueText->SetX(180 - (mHumidValueText->GetWidth() / 2));
    mHumidValueText->SetY(60 - (tbTitle->GetHeight() / 2) - (mHumidValueText->GetHeight() / 2));
    mHumidValueText->SetMaxHeight(mHumidValueText->GetHeight());

    // Background for the value, to erase old value
    mHumidValueBG = new Rectangle(this, 2, "Humid Value Background", 0, mHumidValueText->GetY(), 98, mHumidValueText->GetHeight(), Rectangle::RectType::Filled, 0x0000);
    // Apply centering for x
    mHumidValueBG->SetX(180 - (mHumidValueBG->GetWidth() / 2));

    // "Humid" text positioned centered, under the value text
    mHumidText = new TextBox(this, 1, "Humid Text", 0, mHumidValueText->GetY() + mHumidValueText->GetHeight() + 5, "Humidity (%)", 0, 0, 0xFFFF, 0x0000, 1);
    // Apply centering for x
    mHumidText->SetX(180 - (mHumidText->GetWidth() / 2));

    // Border for Humid value and text
    mHumidBorder = new Rectangle(this, 2, "Humid Border", 0, mHumidValueText->GetY() - 5, 100, mHumidValueText->GetHeight() + mHumidText->GetHeight() + 15, Rectangle::RectType::Outline, 0x0000, 0xFFFF);
    // Apply centering for x
    mHumidBorder->SetX(180 - (mHumidBorder->GetWidth() / 2));

    // --------------------------

    // Big, centered number
    mPresValueText = new TextBox(this, 1, "Pres Value", 0, 0, "?", 0, 0, 0xFFFF, 0x0000, 2);
    // Apply centering
    mPresValueText->SetX(60 - (mPresValueText->GetWidth() / 2));
    mPresValueText->SetY(180 - (tbTitle->GetHeight() / 2) - (mPresValueText->GetHeight() / 2));
    mPresValueText->SetMaxHeight(mPresValueText->GetHeight());

    // Background for the value, to erase old value
    mPresValueBG = new Rectangle(this, 2, "Pres Value Background", 0, mPresValueText->GetY(), 98, mPresValueText->GetHeight(), Rectangle::RectType::Filled, 0x0000);
    // Apply centering for x
    mPresValueBG->SetX(60 - (mPresValueBG->GetWidth() / 2));

    // "Pres" text positioned centered, under the value text
    mPresText = new TextBox(this, 1, "Pres Text", 0, mPresValueText->GetY() + mPresValueText->GetHeight() + 5, "Pressure (Pa)", 0, 0, 0xFFFF, 0x0000, 1);
    // Apply centering for x
    mPresText->SetX(60 - (mPresText->GetWidth() / 2));

    // Border for Pres value and text
    mPresBorder = new Rectangle(this, 2, "Pres Border", 0, mPresValueText->GetY() - 5, 100, mPresValueText->GetHeight() + mPresText->GetHeight() + 15, Rectangle::RectType::Outline, 0x0000, 0xFFFF);
    // Apply centering for x
    mPresBorder->SetX(60 - (mPresBorder->GetWidth() / 2));

    // --------------------------

    // Big, centered number
    mAltValueText = new TextBox(this, 1, "Alt Value", 0, 0, "?", 0, 0, 0xFFFF, 0x0000, 2);
    // Apply centering
    mAltValueText->SetX(180 - (mAltValueText->GetWidth() / 2));
    mAltValueText->SetY(180 - (tbTitle->GetHeight() / 2) - (mAltValueText->GetHeight() / 2));
    mAltValueText->SetMaxHeight(mAltValueText->GetHeight());

    // Background for the value, to erase old value
    mAltValueBG = new Rectangle(this, 2, "Alt Value Background", 0, mAltValueText->GetY(), 98, mAltValueText->GetHeight(), Rectangle::RectType::Filled, 0x0000);
    // Apply centering for x
    mAltValueBG->SetX(180 - (mAltValueBG->GetWidth() / 2));

    // "Alt" text positioned centered, under the value text
    mAltText = new TextBox(this, 1, "Alt Text", 0, mAltValueText->GetY() + mAltValueText->GetHeight() + 5, "Altitude (m)", 0, 0, 0xFFFF, 0x0000, 1);
    // Apply centering for x
    mAltText->SetX(180 - (mAltText->GetWidth() / 2));

    // Border for Alt value and text
    mAltBorder = new Rectangle(this, 2, "Alt Border", 0, mAltValueText->GetY() - 5, 100, mAltValueText->GetHeight() + mAltText->GetHeight() + 15, Rectangle::RectType::Outline, 0x0000, 0xFFFF);
    // Apply centering for x
    mAltBorder->SetX(180 - (mAltBorder->GetWidth() / 2));

    // --------------------------

    // "Sensor Not Connected" text, displayed when the sensor is not connected
    mBMEStatus = new TextBox(this, 1, "BME Status", 0, 0, "BME Disconnected", 0, 0, 0xF800, 0x0000, 1);
    // Apply centering for x
    mBMEStatus->SetX(120 - (mBMEStatus->GetWidth() / 2));
    // Apply centering for y
    mBMEStatus->SetY(120 - (mBMEStatus->GetHeight() / 2));
    // Disable (for now)
    mBMEStatus->SetIsActive(false);

    // Background
    Rectangle* bg = new Rectangle(this, 15, "BME Test Background", 0, 0, 240, 240, Rectangle::RectType::Filled, 0x0000);
}

void BMETest::Unload() {

}

void BMETest::ProcessInput() {
    mTemp[1] = mTemp[0];
    mConnected = Sensors::GetTemp(mTemp[0]);

    mHumid[1] = mHumid[0];
    mConnected = Sensors::GetHumidity(mHumid[0]);

    mPres[1] = mPres[0];
    mConnected = Sensors::GetPressure(mPres[0]);

    mAlt[1] = mAlt[0];
    mConnected = Sensors::GetAltitude(mAlt[0]);

    mBackPressed[1] = mBackPressed[0];
    mBackPressed[0] = Sensors::GetButtonCenter();
}

bool cstrCompare(const char* a, const char* b, size_t size) {
    Serial.print("Comparing '"); Serial.print(a); Serial.print("' to '"); Serial.print(b); Serial.print("': ");
    for(size_t i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            Serial.println("False");
            return false;
        }
    }
    Serial.println("True");
    return true;
}

void BMETest::Update(float deltaTime) {
    // Update the Temp value text
    if (mTemp[0] != mTemp[1]) {
        char vstr[6];
        float_to_cstring(mTemp[0], 6, 2, vstr);
        if (!cstrCompare(vstr, mTempValueText->GetText(), 6)) {
            mTempValueText->SetText(vstr);
            mTempValueText->UpdateCanvas();
            // Reapply x centering
            mTempValueText->SetX(60 - (mTempValueText->GetWidth() / 2));
            // Reapply the background
            mTempValueBG->SetIsActive(true);
        }
    }

    // Update the Humid value text
    if (mHumid[0] != mHumid[1]) {
        char vstr[6];
        float_to_cstring(mHumid[0], 6, 2, vstr);
        if (!cstrCompare(vstr, mHumidValueText->GetText(), 6)) {
            mHumidValueText->SetText(vstr);
            mHumidValueText->UpdateCanvas();
            // Reapply x centering
            mHumidValueText->SetX(180 - (mHumidValueText->GetWidth() / 2));
            // Reapply the background
            mHumidValueBG->SetIsActive(true);
        }
    }

    // Update the Pres value text
    if (mPres[0] != mPres[1]) {
        char vstr[6];
        float_to_cstring(mPres[0], 6, 1, vstr);
        if (!cstrCompare(vstr, mPresValueText->GetText(), 6)) {
            mPresValueText->SetText(vstr);
            mPresValueText->UpdateCanvas();
            // Reapply x centering
            mPresValueText->SetX(60 - (mPresValueText->GetWidth() / 2));
            // Reapply the background
            mPresValueBG->SetIsActive(true);
        }
    }

    // Update the Alt value text
    if (mAlt[0] != mAlt[1]) {
        char vstr[6];
        float_to_cstring(mAlt[0], 6, 1, vstr);
        if (!cstrCompare(vstr, mAltValueText->GetText(), 6)) {
            mAltValueText->SetText(vstr);
            mAltValueText->UpdateCanvas();
            // Reapply x centering
            mAltValueText->SetX(180 - (mAltValueText->GetWidth() / 2));
            // Reapply the background
            mAltValueBG->SetIsActive(true);
        }
    }

    // If the sensor is disconnected, activate the status text
    mBMEStatus->SetIsActive(!mConnected);

    // Check for leading edge press on back button
    if (mBackPressed[0] && mBackPressed[0] != mBackPressed[1]) {
        // Go back to the options menu
        mGame->LoadScene(new OptionsMenu(mGame));
    }
}