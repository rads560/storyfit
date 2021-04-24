#include "drawable.h"

#include "scene.h"

Drawable::Drawable(Scene* scene, int8_t priority)
    :mScene(scene),
     mIsActive(true),
     mUpdate(true),
     mPriority(priority) {
    // Add to the game's drawable list
    mScene->AddDrawable(this);
}

Drawable::~Drawable() {
    // Remove from the game's drawable list
    mScene->RemoveDrawable(this);
}