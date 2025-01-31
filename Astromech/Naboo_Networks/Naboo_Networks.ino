#include <Arduino.h>
#include "src/spi1_driver.h"
#include "src/spi2_driver.h"
#include "src/esp_now_manager.h"

SPI1Driver driver1;
SPI2Driver driver2;
ESPNowManager espNow;

const char* ssid = "eduroam";
const char* username = "cwu81@calpoly.edu";
const char* password = "bJjtKG$k5OGtPY8Q";

uint8_t receiverAddress[] = {0xF0, 0x9E, 0x9E, 0x21, 0xEC, 0x8C}; // Replace with actual MAC address


typedef struct example_message {
  int data;
} example_message;

example_message myData;

void setup() {
    Serial.begin(115200);
    driver1.init();
    driver2.init();
    espNow.init(ssid, username, password);
    Serial.println("Mac Address: ");
    Serial.println(WiFi.macAddress());

    myData.data = 10;

}

void loop() {
  // espNow.sendData(receiverAddress, (uint8_t*)&myData, sizeof(myData));

//     const char *message = "Hello";
//     uint8_t data[] = {0x01, 0x02, 0x03, 0x04};

//     driver1.send(message);
//     Serial.printf("Sent from SPI1: %s\n", message);

//     driver1.send(data, sizeof(data));
//     Serial.printf("Sent from SPI1: 0x%02x 0x%02x 0x%02x 0x%02x\n", data[0], data[1], data[2], data[3]);

// //    if (SPIDriverBase::interrupt_flag) {
// //        Serial.println("Interrupt occurred!");
// //        SPIDriverBase::interrupt_flag = false;
// //    }

//     driver2.send(message);
//     Serial.printf("Sent from SPI2: %s\n", message);

//     driver2.send(data, sizeof(data));
//     Serial.printf("Sent from SPI2: 0x%02x 0x%02x 0x%02x 0x%02x\n", data[0], data[1], data[2], data[3]);

//     delay(10);
}
