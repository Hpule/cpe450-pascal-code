#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "i2c_driver_base.h"

class I2CDriver : public I2CDriverBase {
public:
    I2CDriver();
    void init() override;
    void test_i2c_pins(int sda, int scl, int clockSpeed, int address);
};

#endif // I2C_DRIVER_ESP32S3_H