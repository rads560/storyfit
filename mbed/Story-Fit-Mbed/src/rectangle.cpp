#include "rectangle.h"

#include "scene.h"

Rectangle::Rectangle(Scene* scene, int8_t priority, const char* name, int16_t x, int16_t y, uint16_t width, uint16_t height, RectType type)
    : Drawable(scene, priority, name),
      mPosX(x),
      mPosY(y),
      mWidth(width),
      mHeight(height),
      mType(type) {
    mUpdate = true;
    mIsActive = true;
}

void Rectangle::Draw(Adafruit_GFX &tft) {
  if (mUpdate) {
    switch(mType) {
      case RectType::Filled:
        if (mRadius == 0) {
          tft.fillRect(mPosX, mPosY, mWidth, mHeight, mColor);
        }
        else {
          tft.fillRoundRect(mPosX, mPosY, mWidth, mHeight, mRadius, mColor);
        }
        break;
      case RectType::FilledOutline:
        if (mRadius == 0) {
          tft.fillRect(mPosX, mPosY, mWidth, mHeight, mColor);
          tft.drawRect(mPosX, mPosY, mWidth, mHeight, mOutline);
        }
        else {
          tft.fillRoundRect(mPosX, mPosY, mWidth, mHeight, mRadius, mColor);
          tft.drawRoundRect(mPosX, mPosY, mWidth, mHeight, mRadius, mOutline);
        }
        break;
      case RectType::Outline:
        if (mRadius == 0) {
          tft.drawRect(mPosX, mPosY, mWidth, mHeight, mOutline);
        }
        else {
          tft.drawRoundRect(mPosX, mPosY, mWidth, mHeight, mRadius, mOutline);
        }
        break;
      default: return;
    }
    mUpdate = false;
  }
}