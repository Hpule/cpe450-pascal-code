#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <Wire.h>
#include "i2c_driver_base.h"

class I2CDriver : public I2CDriverBase {
public:
    I2CDriver();
    void init() override;
    void transmit(uint8_t address, const char *str);
};

#endif // I2C_DRIVER_H