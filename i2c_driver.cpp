#include "i2c_driver.h"

I2CDriver::I2CDriver() {
    // Set default pins and clock speed for ESP32-S3
    SDA = 40;
    SCL = 41;
    clockSpeed = 100000; // 100 kHz
}

void I2CDriver::init() {
    Wire.begin(SDA, SCL);
    Wire.setClock(clockSpeed);
    Serial.println("I2C Initialized");
}

void I2CDriver::transmit(uint8_t address, const char *str) {
    Wire.beginTransmission(address);
    while (*str) {
        Wire.write(*str++);
    }
    Wire.endTransmission();
    Serial.println("Data Transmitted");
}
