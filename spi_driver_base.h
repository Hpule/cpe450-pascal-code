
#ifndef SPI_DRIVER_BASE_H
#define SPI_DRIVER_BASE_H

#include <Arduino.h>
#include <SPI.h>

// base class for SPI drivers
class SPIDriverBase {
public:
    SPIDriverBase();

    // for extending classes to override
    virtual ~SPIDriverBase();

    // for extending classes to override
    virtual void init() = 0;

    // send data over SPI
    void send(const uint8_t *data, size_t len);

    // send string over SPI
    void send(const char *str);

    // tried to implement interrupt handling but it didn't work with overriding
    // virtual void IRAM_ATTR gpio_isr_handler() = 0;
    // static volatile bool interrupt_flag;

    // void init_gpio_interrupt(uint8_t gpio_num);

protected:
    uint8_t PIN_NUM_MISO;
    uint8_t PIN_NUM_MOSI;
    uint8_t PIN_NUM_CLK;
    uint8_t PIN_NUM_CS;
    static const int PARALLEL_LINES = 16;
    uint8_t interrupt_pin;
};

#endif // SPI_DRIVER_BASE_H
