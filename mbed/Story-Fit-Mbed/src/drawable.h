class Drawable {
    public:
        Drawable(class Game* game);
        ~Drawable();

        virtual void Draw(class Adafruit_ST7789* tft);

        int8_t GetPriority() { return mPriority; }
        void SetPriority(uint8_t priority) { mPriority = priority; }
    protected:
        class Game* mGame;
        bool mIsActive;
        int8_t mPriority;
    private:

}