/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/

#include "drawable.h"

class TextBox : public Drawable {
    public:
        TextBox(class Scene* scene, int8_t priority, const char* name, int16_t x, int16_t y, const char* text, uint16_t maxWidth = 0, uint16_t maxHeight = 0, uint16_t textColor = 0xFFFF, uint16_t backgroundColor = 0x0000, uint8_t textSize = 1);
        ~TextBox();

        void SetText(const char* text);    // Must call UpdateCanvas() after calling
        char* GetText() { return mText; }
        void SetX(int16_t x) { mPosX = x; mUpdate = true; }
        void SetY(int16_t y) { mPosY = y; mUpdate = true; }
        int16_t GetX() { return mPosX; }
        int16_t GetY() { return mPosY; }
        uint16_t GetWidth() { return mWidth; }
        uint16_t GetHeight() { return mHeight; }
        void SetMaxWidth(uint16_t w) { mMaxWidth = w; }
        void SetMaxHeight(uint16_t h) { mMaxHeight = h; }
        void SetColor(uint16_t color) { mColor = color; mUpdate = true; }
        void SetBackground(uint16_t bg) { mBackground = bg; mUpdate = true; }
        void SetSize(uint8_t size) { mSize = size; }    // Must call UpdateCanvas() after calling
        void SetWrap(bool wrap) { mWrap = wrap; }    // Must call UpdateCanvas() after calling

        void Draw(Adafruit_GFX &tft) override;
        void UpdateCanvas();
    protected:
        char mText[50];
        int16_t mPosX;
        int16_t mPosY;
        uint16_t mWidth;
        uint16_t mHeight;
        uint16_t mMaxWidth;
        uint16_t mMaxHeight;
        uint16_t mColor;
        uint16_t mBackground;
        uint8_t mSize;
        bool mWrap;
        class GFXcanvas1* mCanvas = nullptr;
    private:
};