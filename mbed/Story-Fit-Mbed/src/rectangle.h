#include "drawable.h"

class Rectangle : public Drawable {
    public:
        enum class RectType {
            Filled,
            FilledOutline,
            Outline
        };
        Rectangle(class Scene* scene, int8_t priority, uint16_t x, uint16_t y, uint16_t width, uint16_t height, RectType type = RectType::Filled);

        void SetX(uint16_t x) { mPosX = x; mUpdate = true; }
        void SetY(uint16_t y) { mPosY = y; mUpdate = true; }
        void SetWidth(uint16_t w) { mWidth = w; mUpdate = true; }
        void SetHeight(uint16_t h) { mHeight = h; mUpdate = true; }
        void SetColor(uint16_t c) { mColor = c; mUpdate = true; }
        void SetRadius(uint16_t r) { mRadius = r; mUpdate = true; }
        void SetOutlineColor(uint16_t c) { mOutline = c; mUpdate = true; }
        void SetType(RectType t) { mType = t; mUpdate = true; }

        void Draw(Adafruit_GFX &tft) override;
    protected:
        uint16_t mPosX = 0;
        uint16_t mPosY = 0;
        uint16_t mWidth = 0;
        uint16_t mHeight = 0;
        uint16_t mColor = 0xFFFF;
        uint16_t mOutline = 0x0000;
        RectType mType = RectType::Filled;

        // Round rectangle params
        uint16_t mRadius = 0;
};