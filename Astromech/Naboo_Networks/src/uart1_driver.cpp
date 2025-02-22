#include "uart1_driver.h"

UART1Driver::UART1Driver() {
    serial = &Serial1; // use Serial1 for UART1
}

void UART1Driver::init(long baud_rate) {
    serial->begin(baud_rate);
}

void UART1Driver::send(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        serial->write(data[i]);
    }
}

uint8_t* UART1Driver::receive() {
    static uint8_t buffer[256];
    static size_t buffer_index = 0;

    while (serial->available() > 0) {
        buffer[buffer_index] = serial->read();
        buffer_index++;
    }

    return buffer;
}


// uint8_t sample[] = {0x01, 0x02, 0x03, 0x04};
// UART1Driver uart1;
// uart1.init(9600);
// uart1.send(sample, sizeof(sample));
