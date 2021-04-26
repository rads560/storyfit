#include "storyfit.h"

// #include <Adafruit_GFX.h>       // Core graphics library
#include <Adafruit_ST7789.h>    // Display library
#include <Fonts/romulus9pt7b.h>     // Custom font

#include "drawable.h"
#include "scene.h"
#include "sensors.h"
#include "Scenes/Title.Scene.h"

// Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, 30, 29, TFT_RST);
Adafruit_ST7789 tft = Adafruit_ST7789(&SPI1, TFT_CS, TFT_DC, TFT_RST);

Game::Game() {
    Initialize();
}

Game::~Game() {
}

void Game::Initialize() {
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(TFT_LITE, OUTPUT);

    Serial.begin(9600);
    while (!Serial) {
        digitalWrite(LED_R, HIGH);
    }
    Serial.print("TEST TEST TEST\n");
    digitalWrite(LED_R, LOW);

    // Initialize sensors
    Sensors::SensorsInit();

    // Initialize display
    analogWrite(TFT_LITE, 255);
    tft.init(240, 240);
    tft.setRotation(1);
    // Fill with black
    // tft.fillScreen(0x0000);
    tft.fillScreen(0xFFFF);
    uint8_t rtna = 0x01;
    tft.sendCommand(0xC6, &rtna, 1);
    Serial.print("HERE");
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
        //Serial.println("P");
        ProcessInput();
        //Serial.println("U");
        Update();
        //Serial.println("G");
        GenerateOutput();
        //Serial.println("DONE - "); Serial.println(millis());
    }
}

void Game::ProcessInput() {
    // Call Scene.ProcessInput()
    mCurrentScene->ProcessInput();
}

void Game::Update() {
    uint16_t time = millis();
    float deltaTime = 0.0f;
    // See if time overflowed
    if (time < mLastTime) {
        uint16_t maxT = 0;
        maxT--;
        deltaTime = (float)(time + (maxT - mLastTime)) / 1000.0f;
    }
    else {
        deltaTime = (float)(time - mLastTime) / 1000.0f;
    }

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