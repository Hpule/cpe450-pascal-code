#ifndef I2C_DRIVER_BASE_H
#define I2C_DRIVER_BASE_H

#include <Arduino.h>
#include <Wire.h>

class I2CDriverBase {
public:
    I2CDriverBase();
    virtual ~I2CDriverBase();

    // Pure virtual init so that subclasses implement board-specific initialization.
    virtual void init() = 0;

    // Transmit an array of bytes to a given I2C address.
    void transmit(uint8_t address, const uint8_t *data, size_t len);

    // Transmit a string (as bytes) to a given I2C address.
    void transmit(uint8_t address, const char *str);

protected:
    // I2C pins; these can be set by derived classes.
    uint8_t SDA;
    uint8_t SCL;

    // I2C clock speed in Hz (default 100kHz)
    uint32_t clockSpeed;
};

#endif // I2C_DRIVER_BASE_H