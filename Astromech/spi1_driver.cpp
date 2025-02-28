#include "spi1_driver.h"

// temporary pin numbers for testing change later for specific pins
SPI1Driver::SPI1Driver() {
    PIN_NUM_MISO = 38;
    PIN_NUM_MOSI = 37;
    PIN_NUM_CLK = 39;
    PIN_NUM_CS = 36;
}

// initialize SPI1
void SPI1Driver::init() {
    SPI.begin(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CS);
    SPI.setFrequency(1000000);  // 1 MHz
    SPI.setDataMode(SPI_MODE0);
}

// void IRAM_ATTR SPI1Driver::gpio_isr_handler() {
//     interrupt_flag = true;
// }