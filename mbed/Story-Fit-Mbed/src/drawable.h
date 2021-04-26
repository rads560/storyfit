#ifndef STORYFIT_DRAWABLE_H
    #define STORYFIT_DRAWABLE_H
    #include <Arduino.h>
    #include "boards.h"

    #include <Adafruit_ST7789.h>

    class Drawable {
        public:
            Drawable(class Scene* scene, int8_t priority = 0, const char* name = "kek");
            virtual ~Drawable();

            virtual void Draw(Adafruit_GFX &tft) {}

            int8_t GetPriority() { return mPriority; }
            void SetPriority(int8_t priority) { mPriority = priority; }
            bool ShouldDraw() { return mIsActive && mUpdate; }
            void SetIsActive(bool active) { mIsActive = active; mUpdate = true; }

            const char* mName;
        protected:
            bool mIsActive;
            bool mUpdate;
            class Scene* mScene;
            int8_t mPriority;
        private:

    };
#endif