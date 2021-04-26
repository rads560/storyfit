#include "scene.h"

#include "storyfit.h"
#include "drawable.h"

Scene::Scene(Game* game, Scene* parent)
    :mGame(game),
     mParent(parent) {
    // Fill mDrawables with nullptr
    for (size_t i = 0; i < MAX_DRAWABLES; i++) {
        mDrawables[i] = nullptr;
    }
    mDrawablesSize = 0;
}
Scene::~Scene() {
    // Delete all members of mDrawables
    while (mDrawablesSize > 0) {
        delete mDrawables[mDrawablesSize-1];
    }

    if (mGame->GetCurrentScene() == this) {
        mGame->LoadScene(mParent);
    }
}

void Scene::Draw(Adafruit_GFX &tft) {
    // Loop over drawables
    for (int8_t i = mDrawablesSize - 1; i >= 0; i--) {
        if (mDrawables[i]->ShouldDraw() || mUpdateAll) {
            Serial.print("Drawing at i="); Serial.print(i); Serial.print("; priority="); Serial.println(mDrawables[i]->GetPriority());
            if (!mDrawables[i]) {
                Serial.print("Invalid Drawable at position "); Serial.print(i); Serial.print(" in size "); Serial.println(mDrawablesSize);
                digitalWrite(LED_R, HIGH);
                while(1);
            }
            mDrawables[i]->Draw(tft);
        }
    }
    mUpdateAll = false;
}

void Scene::AddDrawable(Drawable* draw) {
    if (mDrawablesSize == 0) {
        mDrawables[0] = draw;
        mDrawablesSize++;
        return;
    }
    int8_t i = 0;
    // Find the correct spot in the array
    for(; i < mDrawablesSize; i++) {
        if (draw->GetPriority() < mDrawables[i]->GetPriority()) {
            break;
        }
    }
    // Shift everything over in the array to make room
    Drawable* temp;
    for(int8_t j = i+1; j < mDrawablesSize + 1 && j < MAX_DRAWABLES; j++) {
        temp = mDrawables[j];
        mDrawables[j] = mDrawables[j-1];
    }
    // Store the new drawable
    mDrawables[i] = draw;
    mDrawablesSize++;
    if (mDrawablesSize > MAX_DRAWABLES) {
        mDrawablesSize = MAX_DRAWABLES;
    }
}

void Scene::RemoveDrawable(Drawable* draw) {
    for (size_t i = 0; i < mDrawablesSize; i++) {
        if (mDrawables[i] == draw) {
            // Swap until all items after have been shifted
            for (; i < MAX_DRAWABLES - 1; i++) {
                mDrawables[i] = mDrawables[i+1];
            }
            mDrawablesSize--;
            return;
        }
        else if (mDrawables[i] == nullptr) {
            return;
        }
    }
}