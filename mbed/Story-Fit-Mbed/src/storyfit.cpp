#include "storyfit.h"

#include <Adafruit_GFX.h>       // Core graphics library
#include <Adafruit_ST7789.h>    // Display library
#include <Adafruit_Sensor.h>    // Unified sensors
#include <Adafruit_BME280.h>    // BME Sensor
#include <Adafruit_LIS3MDL.h>   // Magnetometer
#include <Adafruit_LSM6DS33.h>  // Accelerometer and Gyroscope
#include <Adafruit_APDS9960.h>  // Light, Proximity, and Guesture sensor
#include <math.h>
#include "MAX30105.h"           // Heartrate sensor
#include "heartRate.h"          // Helper methods for heartrate sensor

#include <Fonts/romulus9pt7b.h>     // Custom font

StoryFit::Game()