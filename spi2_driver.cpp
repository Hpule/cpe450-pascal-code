#include "spi2_driver.h"

// temporary pin numbers for testing change later for specific pins
SPI2Driver::SPI2Driver() {
    PIN_NUM_MISO = 13;
    PIN_NUM_MOSI = 11;
    PIN_NUM_CLK = 12;
    PIN_NUM_CS = 21;
}

// initialize SPI2
void SPI2Driver::init() {
    SPI.begin(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CS);
    SPI.setFrequency(1000000);  // 1 MHz
    SPI.setDataMode(SPI_MODE0);
}

// void IRAM_ATTR SPI2Driver::gpio_isr_handler() {
//     interrupt_flag = true;
// }
