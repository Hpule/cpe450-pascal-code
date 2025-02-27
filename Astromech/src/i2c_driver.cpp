#include "i2c_driver.h"

I2CDriver::I2CDriver() {
<<<<<<< Updated upstream:Astromech/src/i2c_driver.cpp
    // Optionally, set custom SDA/SCL pin numbers for your hardware configuration.
    SDA = 9; 
    SCL = 8; 
}

void I2CDriver::init() {
    // Begin I2C communication on the specified pins.
    Wire.begin(SDA, SCL);
    // Set the I2C clock speed.
    Wire.setClock(clockSpeed);
}
=======
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
>>>>>>> Stashed changes:Astromech/Naboo_Networks/src/i2c_driver.cpp
