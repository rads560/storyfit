/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include "scene.h"

class HRMTest : public Scene {
    public:
        HRMTest(class Game* game, Scene* parent = nullptr);
        ~HRMTest();
        void Load();
        void Unload();

        void ProcessInput() override;
        void Update(float deltaTime) override;
    private:
        class TextBox* mBPMValueText = nullptr;
        class Rectangle* mBPMValueBG = nullptr;
        class TextBox* mBPMText = nullptr;
        class Rectangle* mBPMBorder = nullptr;
        class TextBox* mBPMStatus = nullptr;
        class Rectangle* mBPMStatusBorder = nullptr;

        int mBPM[2] = {0, 0};
        bool mConnected = true;
        bool mBackPressed[2] = {false, false};
};