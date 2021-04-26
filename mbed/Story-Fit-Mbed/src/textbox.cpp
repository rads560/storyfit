#include "textbox.h"

#include <Fonts/romulus9pt7b.h>     // Custom font

TextBox::TextBox(Scene* scene, int8_t priority, const char* name, int16_t x, int16_t y, const char* text, uint16_t maxWidth, uint16_t maxHeight, uint16_t textColor, uint16_t backgroundColor, uint8_t textSize)
    : Drawable(scene, priority, name),
      mPosX(x),
      mPosY(y),
      mWidth(0),
      mHeight(0),
      mMaxWidth(maxWidth),
      mMaxHeight(maxHeight),
      mColor(textColor),
      mBackground(backgroundColor),
      mSize(textSize),
      mWrap(true) {
    // Set text
    SetText(text);
    // Update canvas
    UpdateCanvas();
}

TextBox::~TextBox() {

}

void TextBox::Draw(Adafruit_GFX &tft) {
    // Update the canvas if needed
    if (mUpdate) {
        tft.drawBitmap(mPosX, mPosY, mCanvas->getBuffer(), mWidth, mHeight, mColor, mBackground);
        mUpdate = false;
    }
}

void TextBox::UpdateCanvas() {
    if (mCanvas != nullptr) {
        delete mCanvas;
    }
    uint16_t fixedDisplayW = (mWrap)?240-mPosX:240;
    uint16_t fixedDisplayH = (mWrap)?240-mPosY:240;
    uint16_t tW = (mMaxWidth == 0)?fixedDisplayW:mMaxWidth;
    uint16_t tH = (mMaxHeight == 0)?fixedDisplayH:mMaxHeight;
    GFXcanvas1 tempCanvas = GFXcanvas1(tW, tH);
    tempCanvas.setFont(&romulus9pt7b);
    tempCanvas.setTextSize(mSize);
    // Caclulate text bounds
    int16_t x1 = 0, y1 = 0;
    uint16_t w = 0, h = 0;
    tempCanvas.getTextBounds(mText, 0, 0, &x1, &y1, &w, &h);
    // Serial.print("Boundary calulation complete for text '"); Serial.print(mText); Serial.print("'. (x, y, w, h) => ("); Serial.print(x1); Serial.print(", "); Serial.print(y1); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
    mWidth = (tW < w)?tW:w;
    mHeight = (tH < h)?tH:h;
    // Recreate the canvas
    mCanvas = new GFXcanvas1(mWidth, mHeight);
    mCanvas->setFont(&romulus9pt7b);
    mCanvas->setTextSize(mSize);
    // Set cursor to text height
    mCanvas->setCursor(0, -y1);
    mCanvas->print(mText);
    // Signal to update on the next draw
    mUpdate = true;
}

void TextBox::SetText(const char* text) {
    // copy from text to mText
    for (size_t i = 0; i < 50; i++) {
        mText[i] = text[i];
        if (text[i] == '\0') {
            break;
        }
    }
}