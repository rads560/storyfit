/*
*   StoryFit
*   EE459 Spring 2021
*   Team 13
*   Radhika Agrawal and Justin Wilford
*/
#ifndef STORYFIT_SENSORS_H
    #define STORYFIT_SENSORS_H
    #include <Arduino.h>
    #include "boards.h"

    #include <Adafruit_BME280.h>    // BME Sensor
    #include <Adafruit_LIS3MDL.h>   // Magnetometer
    #include <Adafruit_LSM6DS33.h>  // Accelerometer and Gyroscope
    #include <Adafruit_APDS9960.h>  // Light, Proximity, and Guesture sensor
    #include "MAX30105.h"           // Heartrate sensor

    #define HRM_RATE_SIZE 4

    namespace Sensors {
        void SensorsInit();
        void SensorsClose();

        // Return temperature in degrees C
        bool GetTemp(float &t);

        // Returns relative humidity in Percents
        bool GetHumidity(float &h);

        // Returns pressure in Pascals (Pa)
        bool GetPressure(float &p);

        // Returns altitude in Meters (m)
        bool GetAltitude(float &alt);

        bool GetMagnetic(float &x, float &y, float &z);

        bool GetAccelerometer(float &x, float &y, float &z);

        bool GetGyroscope(float &x, float &y, float &z);

        bool GetHeart(int &h);

        bool GetLight(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c);

        bool GetProx(uint8_t &p);

        bool GetButtonLeft();

        bool GetButtonCenter();

        bool GetButtonRight();

        // BME
        extern Adafruit_BME280 BME;
        extern bool BME_Active;
        extern const float SEALEVELPRESSURE_HPA;

        // Heartrate monitor
        extern MAX30105 HRM;
        extern bool HRM_Active;
        extern byte HRM_rates[HRM_RATE_SIZE]; //Array of heart rates
        extern byte HRM_rateSpot;
        extern long HRM_lastBeat; //Time at which the last beat occurred

        extern float beatsPerMinute;
        extern int beatAvg;

        // Magnetometer
        extern Adafruit_LIS3MDL MAG;
        extern bool MAG_Active;

        // IMU - Accelerometer and Gyroscope
        extern Adafruit_LSM6DS33 IMU;
        extern bool IMU_Active;

        // Light, Prox, Guesture
        extern Adafruit_APDS9960 LIT;
        extern bool LIT_Active;

        extern const uint8_t MAX_CONNECT_ATTEMPTS;
        
        bool BME_Ready();
        bool HRM_Ready();
        bool MAG_Ready();
        bool IMU_Ready();
        bool LIT_Ready();
    }
#endif