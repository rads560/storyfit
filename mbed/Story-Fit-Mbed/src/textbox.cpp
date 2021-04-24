#include "textbox.h"

#include <Fonts/romulus9pt7b.h>     // Custom font

TextBox::TextBox(Scene* scene, int8_t priority, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* text)
    : Drawable(scene, priority),
      mText(text),
      mPosX(x),
      mPosY(y),
      mWidth(width),
      mHeight(height),
      mColor(0x0000),
      mBackground(0xFFFF),
      mSize(1),
      mWrap(true),
      mCanvas(width, height) {
    // mCanvas = GFXcanvas1(mWidth, mHeight);
}

TextBox::~TextBox() {

}

void TextBox::Draw(Adafruit_ST7789 &tft) {
    // Update the canvas if needed
    if (mUpdate) {
        UpdateCanvas();
        tft.drawBitmap(mPosX, mPosY, mCanvas.getBuffer(), mWidth, mHeight, mColor, mBackground);
    }
}

void TextBox::UpdateCanvas() {
    
    mCanvas = GFXcanvas1(mWidth, mHeight);
    mCanvas.setFont(&romulus9pt7b);
    mCanvas.setCursor(0, 0);
    mCanvas.setTextSize(mSize);
    mCanvas.setTextWrap(mWrap);
    mCanvas.println();
    mCanvas.println(mText);

    mUpdate = false;
}