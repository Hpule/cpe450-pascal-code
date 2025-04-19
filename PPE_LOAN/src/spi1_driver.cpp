#include "spi1_driver.h"

// temporary pin numbers for testing change later for specific pins
SPI1Driver::SPI1Driver() {
    // PIN_NUM_MISO = 38;
    // PIN_NUM_MOSI = 37;
    // PIN_NUM_CLK = 39;
    // PIN_NUM_CS = 36;
    PIN_NUM_MISO    = 39;
    PIN_NUM_MOSI    = 38;
    PIN_NUM_CLK     = 36;
    PIN_NUM_CS      = 37;
}

// initialize SPI1
void SPI1Driver::init(int freq) {
    SPI.begin(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CS);
    SPI.setFrequency(freq);  // 1 MHz ~1000000
    SPI.setDataMode(SPI_MODE0);
}

// void IRAM_ATTR SPI1Driver::gpio_isr_handler() {
//     interrupt_flag = true;
// }

void SPI1Driver::test_spi_pins(int miso, int mosi, int clk, int cs) {
    // pinMode(miso, INPUT);
    // pinMode(mosi, OUTPUT);
    // pinMode(clk, OUTPUT);
    // pinMode(cs, OUTPUT);

    Serial.println("Starting SPI test");

    digitalWrite(cs, LOW);


    uint8_t random_byte = random(0, 256);
    uint8_t received_byte = SPI.transfer(random_byte);

    Serial.print("Sent: 0x");
    Serial.println(random_byte, HEX);
    Serial.print("Received: 0x");
    Serial.println(received_byte, HEX);

    if (received_byte == random_byte) {
        Serial.println("SPI test passed: MISO reads the message!");
    } else {
        Serial.println("SPI test failed: MISO does not read the message!");
    }

    Serial.println("CS HIGH");

    delay(1000);
}