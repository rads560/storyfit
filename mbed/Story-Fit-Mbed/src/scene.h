#ifndef STORYFIT_SCENE_H
    #define STORYFIT_SCENE_H
    #include <Arduino.h>
    #include "boards.h"

    #include <Adafruit_ST7789.h>

    class Scene {
        public:
            Scene(class Game* game, Scene* parent = nullptr);
            virtual ~Scene();

            virtual void Load() {}
            virtual void Unload() {}
            virtual void ProcessInput() {}
            virtual void Update(float deltaTime) {}
            virtual void Draw(Adafruit_GFX &tft);

            void AddDrawable(class Drawable* draw);
            void RemoveDrawable(class Drawable* draw);
        protected:

            class Game* mGame;
            Scene* mParent;

            static constexpr size_t MAX_DRAWABLES = 15;
            class Drawable* mDrawables[MAX_DRAWABLES];
            size_t mDrawablesSize = 0;
            bool mUpdateAll = false;
        private:
    };
#endif