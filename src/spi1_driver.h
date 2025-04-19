#ifndef SPI1_DRIVER_H
#define SPI1_DRIVER_H

#include "spi_driver_base.h"

// SPI1 driver class
class SPI1Driver : public SPIDriverBase {
public:
    SPI1Driver();
    void init(int freq) override;

    void test_spi_pins(int miso, int mosi, int clk, int cs);
    // void IRAM_ATTR gpio_isr_handler() override;
};

#endif // SPI1_DRIVER_H