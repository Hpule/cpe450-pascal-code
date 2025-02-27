#include "i2c_driver_base.h"
#include <Wire.h>

I2CDriverBase::I2CDriverBase() 
    : SDA(21), SCL(22), clockSpeed(100000) {}

I2CDriverBase::~I2CDriverBase() {}

void I2CDriverBase::transmit(uint8_t address, const uint8_t *data, size_t len) {
    Wire.beginTransmission(address);
    for (size_t i = 0; i < len; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}
