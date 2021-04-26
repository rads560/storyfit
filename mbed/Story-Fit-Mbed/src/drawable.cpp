#include "drawable.h"

#include "scene.h"

Drawable::Drawable(Scene* scene, int8_t priority, const char* name)
    :mScene(scene),
     mIsActive(true),
     mUpdate(true),
     mPriority(priority),
     mName(name) {
    // Add to the game's drawable list
    mScene->AddDrawable(this);
}

Drawable::~Drawable() {
    // Remove from the game's drawable list
    mScene->RemoveDrawable(this);
}