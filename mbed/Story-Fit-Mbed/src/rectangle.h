#include "drawable.h"

class Rectangle : public Drawable {
    public:
        Rectangle(class Scene* scene, int8_t priority, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

        void SetX(uint16_t x) { mPosX = x; mUpdate = true; }
        void SetY(uint16_t y) { mPosY = y; mUpdate = true; }
        void SetWidth(uint16_t w) { mWidth = w; mUpdate = true; }
        void SetHeight(uint16_t h) { mHeight = h; mUpdate = true; }
        void SetColor(uint16_t c) { mColor = c; mUpdate = true; }

        void Draw(Adafruit_ST7789 &tft) override;
    protected:
        uint16_t mPosX;
        uint16_t mPosY;
        uint16_t mWidth;
        uint16_t mHeight;
        uint16_t mColor;
    private:
};