#include <vector>

class Game {
    public:
        Game();
        ~Game();

        void Run();

        void AddDrawable(class Drawable* draw);
        void RemoveDrawable(class Drawable* draw);
    private:
        void Initialize();
        void ProcessInput();
        void Update();
        void GenerateOutput();

        std::vector<class Drawable*> mDrawables();
}