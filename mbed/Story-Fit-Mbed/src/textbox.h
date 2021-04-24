#include "drawable.h"

class TextBox : public Drawable {
    public:
        TextBox(class Scene* scene, int8_t priority, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char* text);
        ~TextBox();

        void SetText(const char* text) { mText = text; mUpdate = true;}
        void SetX(uint16_t x) { mPosX = x; mUpdate = true; }
        void SetY(uint16_t y) { mPosY = y; mUpdate = true; }
        void SetWidth(uint16_t width) { mWidth = width; mUpdate = true; }
        void SetHeight(uint16_t height) { mHeight = height; mUpdate = true; }
        void SetColor(uint16_t color) { mColor = color; mUpdate = true; }
        void SetBackground(uint16_t bg) { mBackground = bg; mUpdate = true; }
        void SetSize(uint8_t size) { mSize = size; mUpdate = true; }
        void SetWrap(bool wrap) { mWrap = wrap; mUpdate = true; }

        void Draw(Adafruit_ST7789 &tft) override;
    protected:
        const char* mText;
        uint16_t mPosX;
        uint16_t mPosY;
        uint16_t mWidth;
        uint16_t mHeight;
        uint16_t mColor;
        uint16_t mBackground;
        uint8_t mSize;
        bool mWrap;
        GFXcanvas1 mCanvas;

        void UpdateCanvas();
    private:
};