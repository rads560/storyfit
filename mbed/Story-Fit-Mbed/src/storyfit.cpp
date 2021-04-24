#include "storyfit.h"

// #include <Adafruit_GFX.h>       // Core graphics library
#include <Adafruit_ST7789.h>    // Display library
#include <Fonts/romulus9pt7b.h>     // Custom font

#include "drawable.h"
#include "scene.h"
#include "sensors.h"
#include "Scenes/Title.Scene.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

Game::Game() {
    Initialize();
}

Game::~Game() {
}

void Game::Initialize() {
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    Serial.begin(9600);
    while (!Serial) {
        digitalWrite(LED_R, HIGH);
    }
    Serial.print("TEST TEST TEST\n");
    digitalWrite(LED_R, LOW);

    // Initialize sensors
    Sensors::SensorsInit();

    // Initialize display
    tft.init(240, 240);
    // Fill with black
    tft.fillScreen(0x0000);

    // Setup first scene (title screen)
    mCurrentScene = nullptr;
    LoadScene(new TitleScene(this, nullptr));
}

void Game::Run() {
    while (true) {
        if (mCurrentScene == nullptr) {
            digitalWrite(LED_R, HIGH);
            break;
        }
        ProcessInput();
        Update();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    // Call Scene.ProcessInput()
    mCurrentScene->ProcessInput();
}

void Game::Update() {
    uint16_t time = millis();
    float deltaTime = (float)(time - mLastTime) / 1000.0f;
    mFrameTime += deltaTime;
    mLastTime = time;

    // Call Scene.Update()
    mCurrentScene->Update(deltaTime);
}

void Game::GenerateOutput() {
    // Call Scene.Draw()
    mCurrentScene->Draw(tft);
}

void Game::LoadScene(Scene* scene) {
    if (mCurrentScene != nullptr) {
        mCurrentScene->Unload();
    }
    if (scene != nullptr) {
        scene->Load();
    }
    mCurrentScene = scene;
}