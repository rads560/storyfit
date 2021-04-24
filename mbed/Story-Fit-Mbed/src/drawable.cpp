#include "drawable.h"
#include "storyfit.h"

Drawable::Drawable(Game* game)
    :mGame(game),
     mIsActive(true),
     mPriority(0) {
    // Add to the game's drawable list
    mGame->AddDrawable(this);
}

Drawable::~Drawable() {
    // Remove from the game's drawable list
    mGame->RemoveDrawable(this);
}