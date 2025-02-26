#include "uart1_driver.h"

UART1Driver::UART1Driver() {
    serial = &Serial0; // use Serial1 for UART1
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
    static uint8_t buffer[256] = {0}; // Static buffer
    memset(buffer, 0, sizeof(buffer));  // Clear buffer before each call

    if (serial->available()) {
        int bytesRead = serial->readBytesUntil('\n', (char*)buffer, sizeof(buffer) - 1); // Read until newline
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';  // Null-terminate the C-style string
        }
    }

    return buffer;
}

// uint8_t sample[] = {0x01, 0x02, 0x03, 0x04};
// UART1Driver uart1;
// uart1.init(9600);
// uart1.send(sample, sizeof(sample));


// uint8_t* receivedData = uart1.receive();
// if (receivedData != nullptr && strlen((char*)receivedData) > 0) {
//     Serial.print("Received: ");
//     Serial.println((char*)receivedData);


// arduino code to send data to esp32
// void setup() {
//     Serial.begin(9600);
// }

// void loop() {
//     uint8_t data = 0x01;
//     Serial.write(data);
//     delay(1000);
// }