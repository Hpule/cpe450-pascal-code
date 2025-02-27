
#include "spi_driver_base.h"

SPIDriverBase::SPIDriverBase() {}

SPIDriverBase::~SPIDriverBase() {
    SPI.end();
}

// send data over SPI
void SPIDriverBase::send(const uint8_t* data, size_t len) {
    // begin transaction with 1 MHz clock, MSB first, SPI mode 0
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    // select the slave
    digitalWrite(PIN_NUM_CS, LOW);

    // send data
    for (size_t i = 0; i < len; i++) {
        SPI.transfer(data[i]);
    }

    // deselect the slave
    digitalWrite(PIN_NUM_CS, HIGH);
    SPI.endTransaction();
}

void SPIDriverBase::send(const char* str) {
    send(reinterpret_cast<const uint8_t*>(str), strlen(str));
}

// tried to implement interrupt handling but it didn't work with overriding

// volatile bool SPIDriverBase::interrupt_flag = false;

// void IRAM_ATTR SPIDriverBase::gpio_isr_handler() {
//     interrupt_flag = true;
// }

// void SPIDriverBase::init_gpio_interrupt(uint8_t gpio_num) {
//     pinMode(gpio_num, INPUT_PULLUP);
//     attachInterrupt(digitalPinToInterrupt(gpio_num), gpio_isr_handler, FALLING);
// }

// void SPIDriverBase::init_gpio_interrupt(uint8_t gpio_num) {
//     interrupt_pin = gpio_num;
//     pinMode(gpio_num, INPUT_PULLUP);
//     attachInterrupt(digitalPinToInterrupt(interrupt_pin), std::bind(&SPIDriverBase::gpio_isr_handler, this), FALLING);

// }
