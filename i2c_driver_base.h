#ifndef I2C_DRIVER_BASE_H
#define I2C_DRIVER_BASE_H

#include <Arduino.h>
#include <Wire.h>

class I2CDriverBase {
public:
    I2CDriverBase();
    virtual ~I2CDriverBase();

    virtual void init() = 0;
    void transmit(uint8_t address, const uint8_t *data, size_t len);

protected:
    uint8_t SDA;
    uint8_t SCL;
    uint32_t clockSpeed;
};

#endif // I2C_DRIVER_BASE_H
