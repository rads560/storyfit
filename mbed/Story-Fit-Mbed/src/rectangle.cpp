#include "rectangle.h"

#include "scene.h"

Rectangle::Rectangle(Scene* scene, int8_t priority, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
    : Drawable(scene, priority),
      mPosX(x),
      mPosY(y),
      mWidth(width),
      mHeight(height),
      mColor(0xFFFF) {
    
}

void Rectangle::Draw(Adafruit_ST7789 &tft) {
  if (mUpdate) {
    tft.fillRect(mPosX, mPosY, mWidth, mHeight, mColor);
  }
}