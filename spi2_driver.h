#ifndef SPI2_DRIVER_H
#define SPI2_DRIVER_H

#include "spi_driver_base.h"

// SPI2 driver class
class SPI2Driver : public SPIDriverBase {
public:
    SPI2Driver();
    void init() override;

    // void IRAM_ATTR gpio_isr_handler() override;
};

#endif // SPI2_DRIVER_H
