#ifndef STORYFIT_DRAWABLE_H
    #define STORYFIT_DRAWABLE_H
    #include <Arduino.h>
    #include "boards.h"

    #include <Adafruit_ST7789.h>

    class Drawable {
        public:
            Drawable(class Scene* scene, int8_t priority = 0);
            virtual ~Drawable();

            virtual void Draw(Adafruit_GFX &tft) {}

            int8_t GetPriority() { return mPriority; }
            void SetPriority(int8_t priority) { mPriority = priority; }
            bool ShouldDraw() { return mIsActive && mUpdate; }
        protected:
            class Scene* mScene;
            bool mIsActive;
            bool mUpdate;
            int8_t mPriority;
        private:

    };
#endif