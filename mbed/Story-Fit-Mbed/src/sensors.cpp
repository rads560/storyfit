#include "sensors.h"

// #include <Adafruit_Sensor.h>    // Unified sensors
#include <math.h>
#include "heartRate.h"          // Helper methods for heartrate sensor

// Variables to store sensor data
// float s_temp = 0.0f;
// float s_humidity = 0.0f;
// float s_pressure = 0.0f;
// float s_altitude = 0.0f;
// float s_compass[3] = {0.0f, 0.0f, 0.0f};
// float s_accel[3] = {0.0f, 0.0f, 0.0f};
// float s_gyro[3] = {0.0f, 0.0f, 0.0f};
// float s_heart = 0.0f;
// uint16_t s_light[4] = {0, 0, 0, 0};
// uint8_t s_prox = 0;
// bool b_left = false;
// bool b_center = false;
// bool b_right = false;

namespace Sensors {
    // BME
    Adafruit_BME280 BME;
    bool BME_Active = false;
    const float SEALEVELPRESSURE_HPA = 1013.25;

    // Heartrate monitor
    MAX30105 HRM;
    bool HRM_Active = false;
    //Increase this for more averaging. 4 is good.
    byte HRM_rates[HRM_RATE_SIZE]; //Array of heart rates
    byte HRM_rateSpot = 0;
    long HRM_lastBeat = 0; //Time at which the last beat occurred

    float beatsPerMinute;
    int beatAvg;

    // Magnetometer
    Adafruit_LIS3MDL MAG;
    bool MAG_Active = false;

    // IMU - Accelerometer and Gyroscope
    Adafruit_LSM6DS33 IMU;
    bool IMU_Active = false;

    // Light, Prox, Guesture
    Adafruit_APDS9960 LIT;
    bool LIT_Active = false;

    const uint8_t MAX_CONNECT_ATTEMPTS = 10;
}

void Sensors::SensorsInit() {
    pinMode(BUTTON_LEFT, INPUT);
    pinMode(BUTTON_CENTER, INPUT);
    pinMode(BUTTON_RIGHT, INPUT);

    BME_Active = BME.begin();

    HRM_Active = HRM.begin(Wire, I2C_SPEED_FAST);
    if (HRM_Active) {
        HRM.setup(); //Configure sensor with default settings
        HRM.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
        HRM.setPulseAmplitudeGreen(0); //Turn off Green LED
    }

    MAG_Active = MAG.begin_I2C();
    if (MAG_Active) {
        MAG.setPerformanceMode(LIS3MDL_MEDIUMMODE);
        MAG.setOperationMode(LIS3MDL_CONTINUOUSMODE);
        MAG.setDataRate(LIS3MDL_DATARATE_155_HZ);
        MAG.setRange(LIS3MDL_RANGE_16_GAUSS);
    }

    IMU_Active = IMU.begin_I2C();
    
    LIT_Active = LIT.begin();
    if (LIT_Active) {
        LIT.enableProximity(true);
        LIT.enableColor(true);
    }
}

void Sensors::SensorsClose() {
    return;
}

// Return temperature in degrees C
bool Sensors::GetTemp(float &t) {
    // Make sure BME is connected
    if (!BME_Ready()) {
        return false;
    }
    sensors_event_t e;
    BME.getTemperatureSensor()->getEvent(&e);
    t = e.temperature;
    return true;
}

// Returns relative humidity in Percents
bool Sensors::GetHumidity(float &h) {
    // Make sure BME is connected
    if (!BME_Ready()) {
        return false;
    }
    sensors_event_t e;
    BME.getHumiditySensor()->getEvent(&e);
    h = e.relative_humidity;
    return true;
}

// Returns pressure in Pascals (Pa)
bool Sensors::GetPressure(float &p) {
    // Make sure BME is connected
    if (!BME_Ready()) {
        return false;
    }
    sensors_event_t e;
    BME.getPressureSensor()->getEvent(&e);
    p = e.pressure;
    return true;
}

// Returns altitude in Meters (m)
bool Sensors::GetAltitude(float &alt) {
    // Make sure BME is connected
    if (!BME_Ready()) {
        return false;
    }
    alt = BME.readAltitude(SEALEVELPRESSURE_HPA);
    return true;
}

bool Sensors::GetMagnetic(float &x, float &y, float &z) {
    // Make sure MAG is connected
    if (!MAG_Ready()) {
        return false;
    }
    sensors_event_t e;
    MAG.getEvent(&e);
    x = e.magnetic.x;
    y = e.magnetic.y;
    z = e.magnetic.z;
    return true;
}

bool Sensors::GetAccelerometer(float &x, float &y, float &z) {
    // Make sure IMU is connected
    if (!IMU_Ready()) {
        return false;
    }
    sensors_event_t e;
    IMU.getEvent(&e, nullptr, nullptr);
    x = e.acceleration.x;
    y = e.acceleration.y;
    z = e.acceleration.z;
    return true;
}

bool Sensors::GetGyroscope(float &x, float &y, float &z) {
    // Make sure IMU is connected
    if (!IMU_Ready()) {
        return false;
    }
    sensors_event_t e;
    IMU.getEvent(nullptr, &e, nullptr);
    x = e.gyro.x;
    y = e.gyro.y;
    z = e.gyro.z;
    return true;
}

bool Sensors::GetHeart(int &h) {
    // Make sure HRM is connected
    if (!HRM_Ready()) {
        return false;
    }
    long irValue = HRM.getIR();

    if (checkForBeat(irValue) == true) {
        //We sensed a beat!
        long delta = millis() - HRM_lastBeat;
        HRM_lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20) {
        HRM_rates[HRM_rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        HRM_rateSpot %= HRM_RATE_SIZE; //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < HRM_RATE_SIZE ; x++)
            beatAvg += HRM_rates[x];
        beatAvg /= HRM_RATE_SIZE;
        }
    }
    h = beatAvg;
    return true;
}

bool Sensors::GetLight(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) {
    // Make sure LIT is connected
    if (!LIT_Ready()) {
        return false;
    }
    LIT.getColorData(&r, &g, &b, &c);
    return true;
}

bool Sensors::GetProx(uint8_t &p) {
    p = 0;
    return true;
}

bool Sensors::GetButtonLeft() {
    return digitalRead(BUTTON_LEFT);
}

bool Sensors::GetButtonCenter() {
    return digitalRead(BUTTON_CENTER);
}

bool Sensors::GetButtonRight() {
    return digitalRead(BUTTON_RIGHT);
}

bool Sensors::BME_Ready() {
    if (BME_Active) {
        return true;
    }
    else {
        for (uint8_t i = 0; i < MAX_CONNECT_ATTEMPTS; i++) {
            if(BME.begin()) {
                BME_Active = true;
                return true;
            }
            delay(50);
        }
    }
    return false;
}
bool Sensors::HRM_Ready() {
    if (HRM_Active) {
        return true;
    }
    else {
        for (uint8_t i = 0; i < MAX_CONNECT_ATTEMPTS; i++) {
            if(HRM.begin(Wire, I2C_SPEED_FAST)) {
                HRM.setup(); //Configure sensor with default settings
                HRM.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
                HRM.setPulseAmplitudeGreen(0); //Turn off Green LED
                HRM_Active = true;
                return true;
            }
            delay(50);
        }
    }
    return false;
}
bool Sensors::MAG_Ready() {
    if (MAG_Active) {
        return true;
    }
    else {
        for (uint8_t i = 0; i < MAX_CONNECT_ATTEMPTS; i++) {
            if(MAG.begin_I2C()) {
                MAG.setPerformanceMode(LIS3MDL_MEDIUMMODE);
                MAG.setOperationMode(LIS3MDL_CONTINUOUSMODE);
                MAG.setDataRate(LIS3MDL_DATARATE_155_HZ);
                MAG.setRange(LIS3MDL_RANGE_16_GAUSS);
                MAG_Active = true;
                return true;
            }
            delay(50);
        }
    }
    return false;
}
bool Sensors::IMU_Ready() {
    if (IMU_Active) {
        return true;
    }
    else {
        for (uint8_t i = 0; i < MAX_CONNECT_ATTEMPTS; i++) {
            if(IMU.begin_I2C()) {
                IMU_Active = true;
                return true;
            }
            delay(50);
        }
    }
    return false;
}
bool Sensors::LIT_Ready() {
    if (LIT_Active) {
        return true;
    }
    else {
        for (uint8_t i = 0; i < MAX_CONNECT_ATTEMPTS; i++) {
            if(LIT.begin()) {
                LIT.enableProximity(true);
                LIT.enableColor(true);
                LIT_Active = true;
                return true;
            }
            delay(50);
        }
    }
    return false;
}