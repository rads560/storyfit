#include <Arduino.h>
#include "boards.h"
#include <queue>

class Game {
    public:
        Game();
        ~Game();

        void Run();

        void LoadScene(class Scene* scene);
        class Scene* GetCurrentScene() { return mCurrentScene; }
    private:
        void Initialize();
        void ProcessInput();
        void Update();
        void GenerateOutput();

        uint16_t mLastTime = 0;
        float mFrameTime = 0;

        class Scene* mCurrentScene;
};