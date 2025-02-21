#include "i2c_driver_base.h"
#include <cstring>

I2CDriverBase::I2CDriverBase() 
    : SDA(8),      // default SDA pin; adjust as needed
      SCL(9),      // default SCL pin; adjust as needed
      clockSpeed(100000) // default clock speed: 100 kHz
{
}

I2CDriverBase::~I2CDriverBase() {
    // No specific cleanup required for Wire.
}

void I2CDriverBase::transmit(uint8_t address, const uint8_t *data, size_t len) {
    Wire.beginTransmission(address);
    for (size_t i = 0; i < len; i++) {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}

void I2CDriverBase::transmit(uint8_t address, const char *str) {
    transmit(address, reinterpret_cast<const uint8_t*>(str), strlen(str));
}
