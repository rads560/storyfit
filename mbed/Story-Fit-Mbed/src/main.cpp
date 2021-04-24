#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_APDS9960.h>
#include <math.h>
#include "MAX30105.h"
#include "heartRate.h"

#include <Fonts/romulus9pt7b.h>

//=====================================================================================
//    Pin Definitions
//-------------------------------------------------------------------------------------
// Arduino Uno and EE459 ATMEGA328P
#if defined(__AVR_ATmega328P__)
  #define BOARD_NAME_TEST 0
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
  #define BUZZER        3

// Arduino Nano 33 BLE and Adafruit Clue
#elif defined(__CORTEX_M4)
  #define BOARD_NAME_TEST 1
  // I2C Configuration is automatic
  //    SDA => Pin 18 (A4/D18)
  //    SCL => Pin 19 (A5/D19)
  // SPI Configuration is automatic
  //    SS => Pin 10 (D10)
  //    MOSI => Pin 11 (D11)
  //    MISO => Pin 12 (D12)
  //    SCK => Pin 13 (D13)
  #define TFT_CS        D10
  #define TFT_RST       D9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC        D8
  //#define TFT_MOSI 11  // Data out
  //#define TFT_SCLK 13  // Clock out
  #define BUTTON_LEFT   D5
  #define BUTTON_CENTER D9
  #define BUTTON_RIGHT  D11
  #define LED_R         D6
  #define LED_G         D5
  #define LED_B         D4
  #define BUZZER        D3

  #define NO_DTOSTRF    // The Nano 33 BLE doesn't have support for dtostrf(), so we have to make it ourselves

// Nucleo-F401RE
#elif defined(_VARIANT_ARDUINO_STM32_)
  
  #define BOARD_NAME_TEST 2
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
  #define BUZZER        3
#endif

//=====================================================================================
char* my_dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
#ifdef NO_DTOSTRF
  #define float_to_cstring(val, width, prec, sout) my_dtostrf(val, width, prec, sout)
#else
  #define float_to_cstring(val, width, prec, sout) dtostrf(val, width, prec, sout)
#endif


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

// BME
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

// IMU
Adafruit_LSM6DS33 lsm6ds33;

// Light, Prox, Guesture
Adafruit_APDS9960 apds;

// Game variables
uint16_t lastTime = 0;
uint16_t frameTimer = 1000;
uint16_t deltaTime = 0;
int16_t textHeight = 9;

// Variables to store sensor data
float s_temp = 0.0f;
float s_humidity = 0.0f;
float s_pressure = 0.0f;
float s_altitude = 0.0f;
float s_compass[3] = {0.0f, 0.0f, 0.0f};
float s_accel[3] = {0.0f, 0.0f, 0.0f};
float s_gyro[3] = {0.0f, 0.0f, 0.0f};
float s_heart = 0.0f;
uint16_t s_light[4] = {0, 0, 0, 0};
uint8_t s_prox = 0;
bool b_left = false;
bool b_center = false;
bool b_right = false;

void setup() {
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_CENTER, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  // put your setup code here, to run once:
#ifndef NO_SERIAL
  Serial.begin(9600);
  //while(!Serial);    // time to get serial running
  delay(500);
  Serial.print(F("Story Fit Sensor Test on board type: \n")); Serial.print(BOARD_NAME_TEST); Serial.print(" Name:"); Serial.println(BOARD_NAME);
#endif
#ifdef NRF52_SERIES
  Serial.println("NRF52 Series Enabled");
#endif
#ifdef USE_SPI_DMA
  Serial.println("SPI DMA Enabled");
#endif
#ifdef __SAMD51__
  Serial.println("SAMD51 Enabled");
#endif
#ifdef ARDUINO_SAMD_ZERO
  Serial.println("ARDUINO SAMD ZERO Enabled");
#endif

  // Use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  tft.init(240, 240);           // Init ST7789 240x240
  
  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  //tft.setSPISpeed(8000000);

  // Setup BME280
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  while (!bme.begin()) {
    #ifndef NO_SERIAL
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    #endif
      digitalWrite(LED_R, HIGH);
      delay(10);
  }
  digitalWrite(LED_R, LOW);

  // Initialize heartrate sensor
  while (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
  #ifndef NO_SERIAL
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
  #endif
    digitalWrite(LED_G, HIGH);
    delay(10);
  }
  digitalWrite(LED_G, LOW);

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  // Initialize compass
  while (!lis3mdl.begin_I2C()) {
  #ifndef NO_SERIAL
    Serial.println("Failed to find LIS3MDL chip");
  #endif
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    delay(10);
  }
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_16_GAUSS);

  // Initialize IMU
  while (!lsm6ds33.begin_I2C()) {
  #ifndef NO_SERIAL
    Serial.println("Failed to find LSM6DS33 chip");
  #endif
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_B, HIGH);
    delay(10);
  }
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_B, LOW);

  // Initialize Ligh, Prox, and Guesture sensor
  if(!apds.begin()){
  #ifndef NO_SERIAL
    Serial.println("failed to initialize APDS! Please check your wiring.");
  #endif
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
    delay(10);
  }
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  apds.enableProximity(true);
  apds.enableColor(true);
  
//  Serial.println(F("Initialized"));
  digitalWrite(LED_B, HIGH);

  // StoryFit Splash
  tft.setFont(&romulus9pt7b);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(false);
  tft.setTextSize(2);
  tft.print(
    "\nStoryFit\nSensor Test"
    );
  delay(2000);
  digitalWrite(LED_B, LOW);

  // Clear
  tft.fillScreen(ST77XX_BLACK);

  // Print text
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(false);
  tft.setTextSize(1);
  tft.setCursor(0, textHeight);
  tft.print("DeltaTime:");
  tft.setCursor(0, textHeight*3);
  tft.print("Temp:");
  tft.setCursor(0, textHeight*4);
  tft.print("Humidity:");
  tft.setCursor(0, textHeight*5);
  tft.print("Pressure:");
  tft.setCursor(0, textHeight*6);
  tft.print("Altitude:");
  tft.setCursor(0, textHeight*7);
  tft.print("Compass:");
  tft.setCursor(0, textHeight*9);
  tft.print("Accelerometer:");
  tft.setCursor(0, textHeight*11);
  tft.print("Gyroscope:");
  tft.setCursor(0, textHeight*13);
  tft.print("Heart:");
  tft.setCursor(0, textHeight*14);
  tft.print("Light:");
  tft.setCursor(0, textHeight*15);
  tft.print("Proximity:");
}

// Return temperature in degrees C
float getTemp(sensors_event_t &e) {
  bme.getTemperatureSensor()->getEvent(&e);
  return e.temperature;
}

// Returns relative humidity in Percents
float getHumidity(sensors_event_t &e) {
  bme.getHumiditySensor()->getEvent(&e);
  return e.relative_humidity;
}

// Returns pressure in Pascals (Pa)
float getPressure(sensors_event_t &e) {
  bme.getPressureSensor()->getEvent(&e);
  return e.pressure;
}

// Returns altitude in Meters (m)
float getAltitude() {
  return bme.readAltitude(SEALEVELPRESSURE_HPA);
}

void getCompass(sensors_event_t &e, float &x, float &y, float &z) {
  lis3mdl.getEvent(&e);
  x = e.magnetic.x;
  y = e.magnetic.y;
  z = e.magnetic.z;
}

void getAccelerometer(sensors_event_t &e, float &x, float &y, float &z) {
  lsm6ds33.getEvent(&e, nullptr, nullptr);
  x = e.acceleration.x;
  y = e.acceleration.y;
  z = e.acceleration.z;
}

void getGyroscope(sensors_event_t &e, float &x, float &y, float &z) {
  lsm6ds33.getEvent(nullptr, &e, nullptr);
  x = e.gyro.x;
  y = e.gyro.y;
  z = e.gyro.z;
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
  return beatAvg;
}

void getLight(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) {
  apds.getColorData(&r, &g, &b, &c);
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

void process_input() {
  sensors_event_t event;
  
  // Get data from sensors
  //Serial.println("\ttemp");
  s_temp = getTemp(event);

  //Serial.println("\thumidity");
  s_humidity = getHumidity(event);

  //Serial.println("\tpressure");
  s_pressure = getPressure(event);

  //Serial.println("\taltitude");
  s_altitude = getAltitude();

  //Serial.println("\tcompass");
  getCompass(event, s_compass[0], s_compass[1], s_compass[2]);

  //Serial.println("\taccel");
  getAccelerometer(event, s_accel[0], s_accel[1], s_accel[2]);

  //Serial.println("\tgyro");
  getGyroscope(event, s_gyro[0], s_gyro[1], s_gyro[2]);

  //Serial.println("\theart");
  s_heart = getHeart();

  //Serial.println("\tlight");
  getLight(s_light[0], s_light[1], s_light[2], s_light[3]);

  //Serial.println("\tprox");
  s_prox = getProx();

  //Serial.println("\tleft");
  b_left = getButtonLeft();
  //Serial.println("\tcenter");
  b_center = getButtonCenter();
  //Serial.println("\tright");
  b_right = getButtonRight();
}

void update_game() {
  // Update deltaTime
  uint16_t time = millis();
  deltaTime = time - lastTime;
  frameTimer += deltaTime;
  //Serial.print("time: "); Serial.print(time); Serial.print(" lt: "); Serial.print(lastTime); Serial.print(" dt: "); Serial.print(deltaTime); Serial.print(" ft: "); Serial.println(frameTimer);
  
  lastTime = time;
}

#define TEXT_CHAR_HEIGHT 12
#define TEXT_CHAR_WIDTH 9

#define TEXT_START_CHAR 10
#define TEXT_END_CHAR 20
#define TEXT_LENGTH 9

void draw_text(int16_t x, int16_t y, char* text) {
  int16_t bound_x = 0;
  int16_t bound_y = 0;
  uint16_t bound_w = 0;
  uint16_t bound_h = 0;
  
  tft.setCursor(x, y);
  tft.setTextColor(ST77XX_WHITE);
  tft.getTextBounds(text, x, y, &bound_x, &bound_y, &bound_w, &bound_h);
  tft.fillRect(bound_x, bound_y, bound_w, bound_h, ST77XX_BLACK);
  tft.print(text);
}

void generate_output() {
  // Don't update the screen or send serial data more than once a second
  if (frameTimer >= 1000) {
    frameTimer = 0;
    // Send sensor values over serial
  #ifndef NO_SERIAL
    Serial.println("------------------------------");
    Serial.print("DeltaTime: "); Serial.print(deltaTime); Serial.println(" ms");
    Serial.print("Temp: "); Serial.print(s_temp); Serial.println(" *C");
    Serial.print("Humidity: "); Serial.print(s_humidity); Serial.println("%");
    Serial.print("Pressure: "); Serial.print(s_pressure); Serial.println(" Pa");
    Serial.print("Altitude: "); Serial.print(s_altitude); Serial.println(" m");
    Serial.print("Compass: "); Serial.print(s_compass[0]); Serial.print(" "); Serial.print(s_compass[1]); Serial.print(" "); Serial.println(s_compass[2]);
    Serial.print("Accelerometer: "); Serial.print(s_accel[0]); Serial.print(" "); Serial.print(s_accel[1]); Serial.print(" "); Serial.println(s_accel[2]);
    Serial.print("Gyroscope: "); Serial.print(s_gyro[0]); Serial.print(" "); Serial.print(s_gyro[1]); Serial.print(" "); Serial.println(s_gyro[2]);
    Serial.print("Heart: "); Serial.print(s_heart); Serial.println(" bpm");
    Serial.print("Light: "); Serial.print(s_light[0]); Serial.print(" "); Serial.print(s_light[1]); Serial.print(" "); Serial.print(s_light[2]); Serial.print(" "); Serial.println(s_light[3]);
    Serial.print("Proximity: "); Serial.print(s_prox); Serial.println();
  #endif
    
    // Print sensor values to screen
    char linebuf[21];
    char databuf[9];
    char x_buf[7];
    char y_buf[7];
    char z_buf[7];
    uint16_t line_x = 0;
    uint16_t line_y = textHeight;
    uint16_t cursor_x = 0;
    uint16_t cursor_y = 0;
    uint16_t bound_w = 0;
    uint16_t bound_h = 0;
  
    // DeltaTime
    // tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 0, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    // tft.setCursor(cursor_x, cursor_y);
    // tft.setTextColor(ST77XX_WHITE);
    snprintf(databuf, sizeof(databuf), "%d ms", deltaTime);
    // tft.getTextBounds("DeltaTime: ", line_x, line_y, &cursor_x, &cursor_y, &bound_w, &bound_h);

    // tft.print(databuf);
    draw_text(cursor_x, cursor_y, databuf);
  
    // Temperature
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 2*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 2*textHeight);
    float_to_cstring(s_temp, 4, 2, x_buf);
    snprintf(databuf, sizeof(databuf), "%s *C", x_buf);
    tft.print(databuf);
    
    // Humidity
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 3*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 3*textHeight);
    float_to_cstring(s_humidity, 4, 2, x_buf);
    snprintf(databuf, sizeof(databuf), "%s%%", x_buf);
    tft.print(databuf);
  
    // Pressure
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 4*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 4*textHeight);
    float_to_cstring(s_pressure, 4, 2, x_buf);
    snprintf(databuf, sizeof(databuf), "%sPa", x_buf);
    tft.print(databuf);
  
    
    // Altitude
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 5*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 5*textHeight);
    float_to_cstring(s_altitude, 4, 2, x_buf);
    snprintf(databuf, sizeof(databuf), "%s m", x_buf);
    tft.print(databuf);
  
    // Compass
    tft.fillRect(0, 7*textHeight, 20*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(0, 7*textHeight);
    float_to_cstring(s_compass[0], 4, 2, x_buf);
    float_to_cstring(s_compass[1], 4, 2, y_buf);
    float_to_cstring(s_compass[2], 4, 2, z_buf);
    snprintf(linebuf, sizeof(linebuf), "%s %s %s", x_buf, y_buf, z_buf);
    tft.print(linebuf);
  
    // Accelerometer
    tft.fillRect(0, 9*textHeight, 20*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(0, 9*textHeight);
    float_to_cstring(s_accel[0], 4, 2, x_buf);
    float_to_cstring(s_accel[1], 4, 2, y_buf);
    float_to_cstring(s_accel[2], 4, 2, z_buf);
    snprintf(linebuf, sizeof(linebuf), "%s %s %s", x_buf, y_buf, z_buf);
    tft.print(linebuf);
  
    // Gyroscope
    tft.fillRect(0, 11*textHeight, 20*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(0, 11*textHeight);
    float_to_cstring(s_gyro[0], 4, 2, x_buf);
    float_to_cstring(s_gyro[1], 4, 2, y_buf);
    float_to_cstring(s_gyro[2], 4, 2, z_buf);
    snprintf(linebuf, sizeof(linebuf), "%s %s %s", x_buf, y_buf, z_buf);
    tft.print(linebuf);
  
    // Heart
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 12*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 12*textHeight);
    float_to_cstring(s_heart, 4, 2, x_buf);
    snprintf(databuf, sizeof(databuf), "%s", x_buf);
    tft.print(databuf);
  
    // Light
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 13*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 13*textHeight);
    snprintf(databuf, sizeof(databuf), "%d", s_light[3]);
    tft.print(databuf);
      
    // Prox
    tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 14*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
    tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 14*textHeight);
    float_to_cstring(s_prox, 4, 2, x_buf);
    snprintf(databuf, sizeof(databuf), "%s", x_buf);
    tft.print(databuf);
    
  }

  // Left button pressed
  if (b_left) {
    digitalWrite(LED_R, HIGH);
  }
  else {
    digitalWrite(LED_R, LOW);
  }

  // Center button pressed
  if (b_center) {
    digitalWrite(LED_G, HIGH);
  }
  else {
    digitalWrite(LED_G, LOW);
  }

  // Right button pressed
  if (b_right) {
    digitalWrite(LED_B, HIGH);
  }
  else {
    digitalWrite(LED_B, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  process_input();
  update_game();
  generate_output();
  //delay(350);
}