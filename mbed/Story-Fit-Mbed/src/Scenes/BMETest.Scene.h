/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include "scene.h"

class BMETest : public Scene {
    public:
        BMETest(class Game* game, Scene* parent = nullptr);
        ~BMETest();
        void Load();
        void Unload();

        void ProcessInput() override;
        void Update(float deltaTime) override;
    private:
        class TextBox* mTempValueText = nullptr;
        class Rectangle* mTempValueBG = nullptr;
        class TextBox* mTempText = nullptr;
        class Rectangle* mTempBorder = nullptr;

        class TextBox* mHumidValueText = nullptr;
        class Rectangle* mHumidValueBG = nullptr;
        class TextBox* mHumidText = nullptr;
        class Rectangle* mHumidBorder = nullptr;

        class TextBox* mPresValueText = nullptr;
        class Rectangle* mPresValueBG = nullptr;
        class TextBox* mPresText = nullptr;
        class Rectangle* mPresBorder = nullptr;

        class TextBox* mAltValueText = nullptr;
        class Rectangle* mAltValueBG = nullptr;
        class TextBox* mAltText = nullptr;
        class Rectangle* mAltBorder = nullptr;

        class TextBox* mBMEStatus = nullptr;

        float mTemp[2] = {0.0f, 0.0f};
        float mHumid[2] = {0.0f, 0.0f};
        float mPres[2] = {0.0f, 0.0f};
        float mAlt[2] = {0.0f, 0.0f};
        bool mConnected = true;
        bool mBackPressed[2] = {false, false};
};