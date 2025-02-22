#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include "src/spi1_driver.h"
#include "src/spi2_driver.h"
#include "src/esp_now_manager.h"
#include "src/uart1_driver.h"

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// SPI1Driver driver1;
// SPI2Driver driver2;
// UART1Driver uart1;
// ESPNowManager espNow;

// const char* ssid = "eduroam";
// const char* username = "";
// const char* password = "";

// uint8_t receiverAddress[] = {0xF0, 0x9E, 0x9E, 0x21, 0xEC, 0x8C}; // Replace with actual MAC address

// // Nathan's board MAC address:
// // DC:DA:0C:2A:19:28

// // Christopher's board MAC address:
// // F0:9E:9E:21:EC:8C


// typedef struct example_message {
//   int data;
// } example_message;

// example_message myData;

// void setup() {
//     Serial.begin(115200);
//     driver1.init();
//     driver2.init();
//     // espNow.init(ssid, username, password);
//     // espNow.init_send(receiverAddress);
//     // espNow.init_recv();
//     // Serial.println("Mac Address: ");
//     // Serial.println(WiFi.macAddress());
//     // espNow.readMACAddress();
//     uart1.init(115200);

//     myData.data = 10;

// }

// void loop() {
//   // espNow.sendData(receiverAddress, (uint8_t*)&myData, sizeof(myData));
//   uint8_t sample[] = {0x01, 0x02, 0x03, 0x04};
//   uart1.send(sample, sizeof(sample));

// //     const char *message = "Hello";
// //     uint8_t data[] = {0x01, 0x02, 0x03, 0x04};

// //     driver1.send(message);
// //     Serial.printf("Sent from SPI1: %s\n", message);

// //     driver1.send(data, sizeof(data));
// //     Serial.printf("Sent from SPI1: 0x%02x 0x%02x 0x%02x 0x%02x\n", data[0], data[1], data[2], data[3]);

// // //    if (SPIDriverBase::interrupt_flag) {
// // //        Serial.println("Interrupt occurred!");
// // //        SPIDriverBase::interrupt_flag = false;
// // //    }

// //     driver2.send(message);
// //     Serial.printf("Sent from SPI2: %s\n", message);

// //     driver2.send(data, sizeof(data));
// //     Serial.printf("Sent from SPI2: 0x%02x 0x%02x 0x%02x 0x%02x\n", data[0], data[1], data[2], data[3]);

// //     delay(10);
// }

#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>

// Define TFT pins
#define TFT_CS 10  // Chip Select (SPI)
#define TFT_DC 9   // Command/Data
#define TFT_RST 8  // Reset (can be -1 if tied to Arduino reset)

// Define custom SPI pins (for ESP32 or other microcontrollers)
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

// Initialize SPI object
SPIClass mySPI = SPIClass(HSPI);  // Use HSPI on ESP32

// Initialize TFT with custom SPI
Adafruit_HX8357 tft = Adafruit_HX8357(&mySPI, TFT_DC, TFT_CS, TFT_RST);

void setup() {
  Serial.begin(9600);
  Serial.println("HX8357D Test with Custom SPI!");

  // Initialize custom SPI
  mySPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, TFT_CS);  // Use custom pins

  // Initialize TFT with custom SPI
  tft.begin();

  // Test the display
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(2);
  tft.println("Hello, World!");
}

void loop() {
  // Your loop code here
}