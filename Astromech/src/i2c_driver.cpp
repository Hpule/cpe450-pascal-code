#include "i2c_driver.h"

I2CDriver::I2CDriver() {
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