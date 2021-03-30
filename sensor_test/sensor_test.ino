#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_LIS3MDL.h>

#include <SPI.h>
#include <math.h>
#include "MAX30105.h"
#include "heartRate.h"

//=====================================================================================
//    Pin Definitions
//-------------------------------------------------------------------------------------
// Arduino Uno
#ifdef ARDUINO_AVR_UNO
  // I2C Configuration is automatic
  //    SDA => Pin 18 (A4/D18)
  //    SCL => Pin 19 (A5/D19)
  // SPI Configuration is automatic
  //    SS => Pin 10 (D10)
  //    MOSI => Pin 11 (D11)
  //    MISO => Pin 12 (D12)
  //    SCK => Pin 13 (D13)
  #define TFT_CS        10
  #define TFT_RST       9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC        8
  //#define TFT_MOSI 11  // Data out
  //#define TFT_SCLK 13  // Clock out
  #define BUTTON_LEFT   A0
  #define BUTTON_CENTER A1
  #define BUTTON_RIGHT  A2
  #define LED_R         6
  #define LED_G         5
  #define LED_B         4
#endif

// EE459 ATMega328P
#ifdef ARDUINO_EE459_ATMEGA328P
  #ifndef SMALL_MODE
    #define SMALL_MODE
  #endif
  // I2C Configuration is automatic
  //    SDA => Pin 18 (Physical 27)
  //    SCL => Pin 19 (Physical 28)
  // SPI Configuration is automatic
  //    SS => Pin 10 (Physical 16)
  //    MOSI => Pin 11 (Physical 17)
  //    MISO => Pin 12 (Physical 18)
  //    SCK => Pin 13 (Physical 19)
  #define TFT_CS        10    // Physical 16
  #define TFT_RST       9     // Physical 15
  #define TFT_DC        8     // Physical 14
  //#define TFT_MOSI 11  // Data out
  //#define TFT_SCLK 13  // Clock out
  #define BUTTON_LEFT   A0    // Physical 23
  #define BUTTON_CENTER A1    // Physical 24
  #define BUTTON_RIGHT  A2    // Physical 25
  #define LED_R         6     // Physical 12
  #define LED_G         5     // Physical 11
  #define LED_B         4     // Physical 6
#endif

// Nucleo-F401RE
#ifdef ARDUINO_NUCLEO_F401RE
  // I2C Configuration is automatic
  //    SDA => Pin 14 (PB9/D14)
  //    SCL => Pin 15 (PB8/D15)
  // SPI Configuration is automatic
  //    SS => Pin 10 (PB6/D10)
  //    SS1 => Pin 4 (PB5/D4)
  //    SS2 => Pin 7 (PA8/D7)
  //    SS3 => Pin 8 (PA9/D8)
  //    MOSI => Pin 11 (D11)
  //    MISO => Pin 12 (D12)
  //    SCK => Pin 13 (PA5/D13)
  #define TFT_CS        10
  #define TFT_RST       9     // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC        8
  //#define TFT_MOSI 11  // Data out
  //#define TFT_SCLK 13  // Clock out
  #define BUTTON_LEFT   46    //A0
  #define BUTTON_CENTER 47    //A1
  #define BUTTON_RIGHT  48    //A2
  #define LED_R         6
  #define LED_G         5
  #define LED_B         4
#endif
//=====================================================================================

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Arduino Uno: MOSI = pin 11 and
// SCLK = pin 13. This is the fastest mode of operation and is required if
// using the breakout board's microSD card.

// For 1.14", 1.3", 1.54", and 2.0" TFT with ST7789:
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 11  // Data out
//#define TFT_SCLK 13  // Clock out

// For the ST7789-based displays, we will use this call
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

// Heartrate monitor
MAX30105 particleSensor;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

// Compass
Adafruit_LIS3MDL lis3mdl;

void setup() {
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_CENTER, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  // put your setup code here, to run once:
#ifndef SMALL_MODE
  Serial.begin(9600);
  while(!Serial);    // time to get serial running
  Serial.print(F("Story Fit Sensor Test"));
#endif

  // Use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  tft.init(240, 240);           // Init ST7789 240x240
  
  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  //tft.setSPISpeed(F_CPU/2);

  // Setup BME280
  unsigned status = bme.begin();  
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  if (!status) {
    #ifndef SMALL_MODE
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
    #endif
      digitalWrite(LED_R, HIGH);
      while (1) delay(10);
  }

  // Initialize heartrate sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
  #ifndef SMALL_MODE
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
  #endif
    digitalWrite(LED_G, HIGH);
    while (1);
  }

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  // Initialize compass
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_16_GAUSS);
  
//  Serial.println(F("Initialized"));
  digitalWrite(LED_B, HIGH);

  // StoryFit Splash
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(false);
  tft.setTextSize(3);
  tft.print(
    "StoryFit\nSensor Test"
    );
  delay(2000);
  digitalWrite(LED_B, LOW);

  // Clear
  tft.fillScreen(ST77XX_BLACK);

  // Print text
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(false);
  tft.setTextSize(2);
  tft.print(
    "DeltaTime:\n\nTemp:\nHumidity:\nPressure:\nAltitude:\n\nCompass:\n\nHeart:\n\nLight:\nProximity:\n\nB_L     B_C     B_R"
    );
}

// Return temperature in degrees C
float getTemp() {
  return trunc(bme.readTemperature() * 10) / 10;
}

// Returns relative humidity in Percents
float getHumidity() {
  return trunc(bme.readHumidity() * 10) / 10;
}

// Returns pressure in Pascals (Pa)
float getPressure() {
  return trunc((float)bme.readPressure() / 100) / 10;
}

// Returns altitude in Meters (m)
float getAltitude() {
  return trunc(bme.readAltitude(SEALEVELPRESSURE_HPA) * 10) / 10;
}

void getCompass(float &x, float &y, float &z) {
  lis3mdl.read();
  x = (lis3mdl.x_gauss * 100);
  y = (lis3mdl.y_gauss * 100);
  z = (lis3mdl.z_gauss * 100);
  
}

float getHeart() {
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true) {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
  return beatsPerMinute;
}

uint8_t getLight() {
  return 0;
}

uint8_t getProx() {
  return 0;
}

bool getButtonLeft() {
  return digitalRead(BUTTON_LEFT);
}

bool getButtonCenter() {
  return digitalRead(BUTTON_CENTER);
}

bool getButtonRight() {
  return digitalRead(BUTTON_RIGHT);
}

// Variables to store sensor data
float s_temp;
bool s_temp_ch;
float s_humidity;
bool s_humidity_ch;
float s_pressure;
bool s_pressure_ch;
float s_altitude;
bool s_altitude_ch;
float s_compass[3];
bool s_compass_ch;
float s_heart;
bool s_heart_ch;
uint8_t s_light;
bool s_light_ch;
uint8_t s_prox;
bool s_prox_ch;
bool b_left;
bool b_center;
bool b_right;

void process_input() {
  // Get data from sensors
  float temp = getTemp();
  s_temp_ch = temp != s_temp;
  s_temp = temp;

  float humid = getHumidity();
  s_humidity_ch = humid != s_humidity_ch;
  s_humidity = humid;

  float pres = getPressure();
  s_pressure_ch = pres != s_pressure;
  s_pressure = pres;

  float alti = getAltitude();
  s_altitude_ch = alti != s_altitude;
  s_altitude = alti;

  float x, y, z;
  getCompass(x, y, z);
  s_compass_ch = (x != s_compass[0]) || (y != s_compass[1]) || (z != s_compass[2]);
  s_compass[0] = x;
  s_compass[1] = y;
  s_compass[2] = z;

  float heart = getHeart();
  s_heart_ch = heart != s_heart;
  s_heart = heart;

  int light = getLight();
  s_light_ch = light != s_light;
  s_light = light;

  int prox = getProx();
  s_prox_ch = prox != s_prox;
  s_prox = prox;

  b_left = getButtonLeft();
  b_center = getButtonCenter();
  b_right = getButtonRight();
}

uint16_t lastTime;
uint16_t deltaTime;
String screenText;

void update_game() {
  // Update deltaTime
  uint16_t time = millis();
  deltaTime = time - lastTime;
  lastTime = time;
}

#define TEXT_CHAR_HEIGHT 16
#define TEXT_CHAR_WIDTH 12

#define TEXT_START_CHAR 11
#define TEXT_END_CHAR 20
#define TEXT_LENGTH 8

void generate_output() {
  // Print sensor values
  
  tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 0, TEXT_LENGTH*TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT, ST77XX_BLACK);
  tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(String(deltaTime, DEC) + " ms");

  if (s_temp_ch) {
    // Clear old value
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 2*TEXT_CHAR_HEIGHT, TEXT_LENGTH*TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 2*TEXT_CHAR_HEIGHT);
    tft.print(String(s_temp, 2) + " *C");
  }
  if (s_humidity_ch) {
    // Clear old value
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 3*TEXT_CHAR_HEIGHT, TEXT_LENGTH*TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 3*TEXT_CHAR_HEIGHT);
    tft.print(String(s_humidity, 2) + "%");
  }
  if (s_pressure_ch) {
    // Clear old value
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 4*TEXT_CHAR_HEIGHT, TEXT_LENGTH*TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 4*TEXT_CHAR_HEIGHT);
    tft.print(String(s_pressure, 2) + " Pa");
  }
  if (s_altitude_ch) {
    // Clear old value
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 5*TEXT_CHAR_HEIGHT, TEXT_LENGTH*TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 5*TEXT_CHAR_HEIGHT);
    tft.print(String(s_altitude, 2) + " m");
  }

  if (s_compass_ch) {
    tft.fillRect(0, 8*TEXT_CHAR_HEIGHT, 20*TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT, ST77XX_BLACK);
    tft.setCursor(0, 8*TEXT_CHAR_HEIGHT);
    tft.print(String(s_compass[0], 0) + " " + String(s_compass[1], 0) + " " + String(s_compass[2], 0));
  }

  if (s_heart_ch) {
    // Clear old value
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 9*TEXT_CHAR_HEIGHT, TEXT_LENGTH*TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 9*TEXT_CHAR_HEIGHT);
    tft.print(String(s_heart, 2));
  }

  if (s_light_ch) {
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 11*TEXT_CHAR_HEIGHT);
    tft.print(String(s_light, DEC));
  }
  if (s_prox_ch) {
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 12*TEXT_CHAR_HEIGHT);
    tft.print(String(s_prox, DEC));
  }

  // Left button pressed
  if (b_left) {
    tft.setTextColor(ST77XX_YELLOW);
    digitalWrite(LED_R, HIGH);
  }
  else {
    tft.setTextColor(ST77XX_WHITE);
    digitalWrite(LED_R, LOW);
  }
  tft.setCursor(0, 14*TEXT_CHAR_HEIGHT);
  tft.print("B_L");

  // Center button pressed
  if (b_center) {
    tft.setTextColor(ST77XX_YELLOW);
    digitalWrite(LED_G, HIGH);
  }
  else {
    tft.setTextColor(ST77XX_WHITE);
    digitalWrite(LED_G, LOW);
  }
  tft.setCursor(8*TEXT_CHAR_WIDTH, 14*TEXT_CHAR_HEIGHT);
  tft.print("B_C");

  // Right button pressed
  if (b_right) {
    tft.setTextColor(ST77XX_YELLOW);
    digitalWrite(LED_B, HIGH);
  }
  else {
    tft.setTextColor(ST77XX_WHITE);
    digitalWrite(LED_B, LOW);
  }
  tft.setCursor(16*TEXT_CHAR_WIDTH, 14*TEXT_CHAR_HEIGHT);
  tft.print("B_R");
}

void loop() {
  // put your main code here, to run repeatedly:
  process_input();
  update_game();
  generate_output();
  //delay(350);
}
