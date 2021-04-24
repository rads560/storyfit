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

void Scene::Draw(Adafruit_ST7789 &tft) {
    // Loop over drawables in reverse order to maintain proper priority
    for (int8_t i = mDrawablesSize - 1; i > 0; i--) {
        if (mDrawables[i]->ShouldDraw()) {
            mDrawables[i]->Draw(tft);
        }
    }
}

void Scene::AddDrawable(Drawable* draw) {
    if (mDrawablesSize == 0) {
        mDrawables[0] = draw;
        mDrawablesSize++;
        return;
    }
    for (size_t i = 0; i < mDrawablesSize; i++) {
        if (mDrawables[i]->GetPriority() < draw->GetPriority()) {
            // Swap until all items have been shifted
            Drawable* temp1 = mDrawables[i];
            mDrawables[i] = draw;
            for (i = i + 1; i <= mDrawablesSize; i++) {
                if (mDrawables[i] == nullptr) {
                    mDrawables[i] = temp1;
                    break;
                }
                else {
                    Drawable* temp2 = mDrawables[i];
                    mDrawables[i] = temp1;
                    temp1 = temp2;
                }
            }
            mDrawablesSize++;
            return;
        }
        else if (i == mDrawablesSize - 1) {
            if (mDrawablesSize != MAX_DRAWABLES) {
                mDrawables[mDrawablesSize] = draw;
                mDrawablesSize++;
                return;
            }
            else {
                // If the drawable list is full, overwrite the last one
                delete mDrawables[mDrawablesSize - 1];
                mDrawables[mDrawablesSize] = draw;
            }
        }
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