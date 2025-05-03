// #include "uart2_driver.h"

// // Use HardwareSerial1 for UART1
// HardwareSerial SerialPort2(2);

// UART2Driver::UART2Driver() {
//     serial = &SerialPort2;  // Assign Serial2
// }

// void UART2Driver::init(long baud_rate) {
//     SerialPort2.begin(baud_rate, SERIAL_8N1, RXD2, TXD2);
// }

// void UART2Driver::send(const uint8_t* data, size_t len) {
//     for (size_t i = 0; i < len; i++) {
//         SerialPort2.write(data[i]);
//     }
// }

// uint8_t* UART2Driver::receive() {
//     static uint8_t buffer[256] = {0}; // Static buffer
//     memset(buffer, 0, sizeof(buffer));  // Clear buffer before each call

//     if (SerialPort2.available()) {
//         int bytesRead = SerialPort2.readBytesUntil('\n', (char*)buffer, sizeof(buffer) - 1); // Read until newline
//         if (bytesRead > 0) {
//             buffer[bytesRead] = '\0';  // Null-terminate the C-style string
//         }
//     }

//     return buffer;
// }
