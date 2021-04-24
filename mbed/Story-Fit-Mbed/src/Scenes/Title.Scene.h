#include "scene.h"

class TitleScene : public Scene {
    public:
        TitleScene(class Game* game, Scene* parent = nullptr);
        ~TitleScene();
        void Load();
        void Unload();

        void Update(float deltaTime) override;
    private:
        class TextBox* tbTitle = nullptr;
        class TextBox* tbPrompt = nullptr;
        float mTimer;
        bool mPromptHighlight = false;

        static constexpr float PROMPT_FLASH_PERIOD = 1.0f;
};