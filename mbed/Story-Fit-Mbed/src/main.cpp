#include <Arduino.h>
#include "boards.h"

#include "storyfit.h"

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
// Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 11  // Data out
//#define TFT_SCLK 13  // Clock out

// For the ST7789-based displays, we will use this call
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Game variables
uint16_t lastTime = 0;
uint16_t frameTimer = 1000;
uint16_t deltaTime = 0;
int16_t textHeight = 9;

void setup() {
  Game game = Game();
  game.Run();
//   pinMode(BUTTON_LEFT, INPUT_PULLUP);
//   pinMode(BUTTON_CENTER, INPUT_PULLUP);
//   pinMode(BUTTON_RIGHT, INPUT_PULLUP);

//   pinMode(LED_R, OUTPUT);
//   pinMode(LED_G, OUTPUT);
//   pinMode(LED_B, OUTPUT);
  
//   // put your setup code here, to run once:
// #ifndef NO_SERIAL
//   Serial.begin(9600);
//   //while(!Serial);    // time to get serial running
//   delay(500);
//   Serial.print(F("Story Fit Sensor Test on board type: \n")); Serial.print(BOARD_NAME_TEST); Serial.print(" Name:"); Serial.println(BOARD_NAME);
// #endif
// #ifdef NRF52_SERIES
//   Serial.println("NRF52 Series Enabled");
// #endif
// #ifdef USE_SPI_DMA
//   Serial.println("SPI DMA Enabled");
// #endif
// #ifdef __SAMD51__
//   Serial.println("SAMD51 Enabled");
// #endif
// #ifdef ARDUINO_SAMD_ZERO
//   Serial.println("ARDUINO SAMD ZERO Enabled");
// #endif

  // Use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  // tft.init(240, 240);           // Init ST7789 240x240
  
  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  //tft.setSPISpeed(8000000);
  
//  Serial.println(F("Initialized"));
  // digitalWrite(LED_B, HIGH);

  // StoryFit Splash
  // tft.setFont(&romulus9pt7b);
  // tft.fillScreen(ST77XX_BLACK);
  // tft.setCursor(0, 0);
  // tft.setTextColor(ST77XX_WHITE);
  // tft.setTextWrap(false);
  // tft.setTextSize(2);
  // tft.print(
  //   "\nStoryFit\nSensor Test"
  //   );
  // delay(2000);
  // digitalWrite(LED_B, LOW);

  // Clear
  // tft.fillScreen(ST77XX_BLACK);

  // Print text
  // tft.setTextColor(ST77XX_WHITE);
  // tft.setTextWrap(false);
  // tft.setTextSize(1);
  // tft.setCursor(0, textHeight);
  // tft.print("DeltaTime:");
  // tft.setCursor(0, textHeight*3);
  // tft.print("Temp:");
  // tft.setCursor(0, textHeight*4);
  // tft.print("Humidity:");
  // tft.setCursor(0, textHeight*5);
  // tft.print("Pressure:");
  // tft.setCursor(0, textHeight*6);
  // tft.print("Altitude:");
  // tft.setCursor(0, textHeight*7);
  // tft.print("Compass:");
  // tft.setCursor(0, textHeight*9);
  // tft.print("Accelerometer:");
  // tft.setCursor(0, textHeight*11);
  // tft.print("Gyroscope:");
  // tft.setCursor(0, textHeight*13);
  // tft.print("Heart:");
  // tft.setCursor(0, textHeight*14);
  // tft.print("Light:");
  // tft.setCursor(0, textHeight*15);
  // tft.print("Proximity:");
}

void process_input() {
}

void update_game() {
}

#define TEXT_CHAR_HEIGHT 12
#define TEXT_CHAR_WIDTH 9

#define TEXT_START_CHAR 10
#define TEXT_END_CHAR 20
#define TEXT_LENGTH 9

void draw_text(int16_t x, int16_t y, char* text) {
  // int16_t bound_x = 0;
  // int16_t bound_y = 0;
  // uint16_t bound_w = 0;
  // uint16_t bound_h = 0;
  
  // tft.setCursor(x, y);
  // tft.setTextColor(ST77XX_WHITE);
  // tft.getTextBounds(text, x, y, &bound_x, &bound_y, &bound_w, &bound_h);
  // tft.fillRect(bound_x, bound_y, bound_w, bound_h, ST77XX_BLACK);
  // tft.print(text);
}

void generate_output() {
  // // Don't update the screen or send serial data more than once a second
  // if (frameTimer >= 1000) {
  //   frameTimer = 0;
  //   // Send sensor values over serial
  // #ifndef NO_SERIAL
  //   Serial.println("------------------------------");
  //   Serial.print("DeltaTime: "); Serial.print(deltaTime); Serial.println(" ms");
  //   Serial.print("Temp: "); Serial.print(s_temp); Serial.println(" *C");
  //   Serial.print("Humidity: "); Serial.print(s_humidity); Serial.println("%");
  //   Serial.print("Pressure: "); Serial.print(s_pressure); Serial.println(" Pa");
  //   Serial.print("Altitude: "); Serial.print(s_altitude); Serial.println(" m");
  //   Serial.print("Compass: "); Serial.print(s_compass[0]); Serial.print(" "); Serial.print(s_compass[1]); Serial.print(" "); Serial.println(s_compass[2]);
  //   Serial.print("Accelerometer: "); Serial.print(s_accel[0]); Serial.print(" "); Serial.print(s_accel[1]); Serial.print(" "); Serial.println(s_accel[2]);
  //   Serial.print("Gyroscope: "); Serial.print(s_gyro[0]); Serial.print(" "); Serial.print(s_gyro[1]); Serial.print(" "); Serial.println(s_gyro[2]);
  //   Serial.print("Heart: "); Serial.print(s_heart); Serial.println(" bpm");
  //   Serial.print("Light: "); Serial.print(s_light[0]); Serial.print(" "); Serial.print(s_light[1]); Serial.print(" "); Serial.print(s_light[2]); Serial.print(" "); Serial.println(s_light[3]);
  //   Serial.print("Proximity: "); Serial.print(s_prox); Serial.println();
  // #endif
    
  //   // Print sensor values to screen
  //   char linebuf[21];
  //   char databuf[9];
  //   char x_buf[7];
  //   char y_buf[7];
  //   char z_buf[7];
  //   uint16_t line_x = 0;
  //   uint16_t line_y = textHeight;
  //   uint16_t cursor_x = 0;
  //   uint16_t cursor_y = 0;
  //   uint16_t bound_w = 0;
  //   uint16_t bound_h = 0;
  
  //   // DeltaTime
  //   // tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 0, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   // tft.setCursor(cursor_x, cursor_y);
  //   // tft.setTextColor(ST77XX_WHITE);
  //   snprintf(databuf, sizeof(databuf), "%d ms", deltaTime);
  //   // tft.getTextBounds("DeltaTime: ", line_x, line_y, &cursor_x, &cursor_y, &bound_w, &bound_h);

  //   // tft.print(databuf);
  //   draw_text(cursor_x, cursor_y, databuf);
  
  //   // Temperature
  //   tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 2*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 2*textHeight);
  //   float_to_cstring(s_temp, 4, 2, x_buf);
  //   snprintf(databuf, sizeof(databuf), "%s *C", x_buf);
  //   tft.print(databuf);
    
  //   // Humidity
  //   tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 3*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 3*textHeight);
  //   float_to_cstring(s_humidity, 4, 2, x_buf);
  //   snprintf(databuf, sizeof(databuf), "%s%%", x_buf);
  //   tft.print(databuf);
  
  //   // Pressure
  //   tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 4*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 4*textHeight);
  //   float_to_cstring(s_pressure, 4, 2, x_buf);
  //   snprintf(databuf, sizeof(databuf), "%sPa", x_buf);
  //   tft.print(databuf);
  
    
  //   // Altitude
  //   tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 5*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 5*textHeight);
  //   float_to_cstring(s_altitude, 4, 2, x_buf);
  //   snprintf(databuf, sizeof(databuf), "%s m", x_buf);
  //   tft.print(databuf);
  
  //   // Compass
  //   tft.fillRect(0, 7*textHeight, 20*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(0, 7*textHeight);
  //   float_to_cstring(s_compass[0], 4, 2, x_buf);
  //   float_to_cstring(s_compass[1], 4, 2, y_buf);
  //   float_to_cstring(s_compass[2], 4, 2, z_buf);
  //   snprintf(linebuf, sizeof(linebuf), "%s %s %s", x_buf, y_buf, z_buf);
  //   tft.print(linebuf);
  
  //   // Accelerometer
  //   tft.fillRect(0, 9*textHeight, 20*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(0, 9*textHeight);
  //   float_to_cstring(s_accel[0], 4, 2, x_buf);
  //   float_to_cstring(s_accel[1], 4, 2, y_buf);
  //   float_to_cstring(s_accel[2], 4, 2, z_buf);
  //   snprintf(linebuf, sizeof(linebuf), "%s %s %s", x_buf, y_buf, z_buf);
  //   tft.print(linebuf);
  
  //   // Gyroscope
  //   tft.fillRect(0, 11*textHeight, 20*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(0, 11*textHeight);
  //   float_to_cstring(s_gyro[0], 4, 2, x_buf);
  //   float_to_cstring(s_gyro[1], 4, 2, y_buf);
  //   float_to_cstring(s_gyro[2], 4, 2, z_buf);
  //   snprintf(linebuf, sizeof(linebuf), "%s %s %s", x_buf, y_buf, z_buf);
  //   tft.print(linebuf);
  
  //   // Heart
  //   tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 12*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 12*textHeight);
  //   float_to_cstring(s_heart, 4, 2, x_buf);
  //   snprintf(databuf, sizeof(databuf), "%s", x_buf);
  //   tft.print(databuf);
  
  //   // Light
  //   tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 13*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 13*textHeight);
  //   snprintf(databuf, sizeof(databuf), "%d", s_light[3]);
  //   tft.print(databuf);
      
  //   // Prox
  //   tft.fillRect(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 14*textHeight, TEXT_LENGTH*TEXT_CHAR_WIDTH, textHeight, ST77XX_BLACK);
  //   tft.setCursor(TEXT_START_CHAR*TEXT_CHAR_WIDTH, 14*textHeight);
  //   float_to_cstring(s_prox, 4, 2, x_buf);
  //   snprintf(databuf, sizeof(databuf), "%s", x_buf);
  //   tft.print(databuf);
    
  // }

  // // Left button pressed
  // if (b_left) {
  //   digitalWrite(LED_R, HIGH);
  // }
  // else {
  //   digitalWrite(LED_R, LOW);
  // }

  // // Center button pressed
  // if (b_center) {
  //   digitalWrite(LED_G, HIGH);
  // }
  // else {
  //   digitalWrite(LED_G, LOW);
  // }

  // // Right button pressed
  // if (b_right) {
  //   digitalWrite(LED_B, HIGH);
  // }
  // else {
  //   digitalWrite(LED_B, LOW);
  // }
}

void loop() {
  // put your main code here, to run repeatedly:
  // process_input();
  // update_game();
  // generate_output();
  //delay(350);
}