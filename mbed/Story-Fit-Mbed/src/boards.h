/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#include <Arduino.h>

#ifndef STORYFIT_BOARDS_H
    #define STORYFIT_BOARDS_H
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
    #define TFT_LITE      7
    #define STATUS_LED    LED_BUILTIN
    //#define TFT_MOSI 11  // Data out
    //#define TFT_SCLK 13  // Clock out
    #define BUTTON_LEFT   46    //A0
    #define BUTTON_CENTER 47    //A1
    #define BUTTON_RIGHT  48    //A2
    #define LED_R         5
    #define LED_G         4
    #define LED_B         6
    #define BUZZER        3

    // Arduino Nano 33 BLE and Adafruit Clue
    #else
    #define CLUE
    #define BOARD_NAME_TEST 3
    // I2C Configuration is automatic
    //    SDA => Pin 20
    //    SCL => Pin 19
    // SPI Configuration is automatic
    //    SS => Pin 16
    //    MOSI => Pin 15
    //    MISO => Pin 14
    //    SCK => Pin 13
    #define TFT_CS        31
    #define TFT_RST       33 // Or set to -1 and connect to Arduino RESET pin
    #define TFT_DC        32
    #define TFT_LITE      34
    //#define TFT_MOSI 11  // Data out
    //#define TFT_SCLK 13  // Clock out
    #define BUTTON_LEFT   2
    #define BUTTON_CENTER 5 // not built in
    #define BUTTON_RIGHT  11
    #define LED_R         4
    #define LED_G         8
    #define LED_B         9
    #define NEOPIXEL      18
    #define STATUS_LED    17
    #define BUZZER        46

    #define NO_DTOSTRF    // The Nano 33 BLE doesn't have support for dtostrf(), so we have to make it ourselves
    #endif

    #ifdef NO_DTOSTRF
        char* my_dtostrf (double val, signed char width, unsigned char prec, char *sout);
        #define float_to_cstring(val, width, prec, sout) my_dtostrf(val, width, prec, sout)
    #else
        #define float_to_cstring(val, width, prec, sout) dtostrf(val, width, prec, sout)
    #endif
#endif