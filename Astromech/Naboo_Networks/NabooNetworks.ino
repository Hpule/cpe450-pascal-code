#include <Arduino.h>
#include "src/ScreenManager.h"
#include "src/uart1_driver.h"

// Pin definitions
#define TFT_CS   36
#define TFT_DC   35
#define TFT_RST  -1

// Create ScreenManager object
ScreenManager screenManager(TFT_CS, TFT_DC, TFT_RST);
UART1Driver uart1;
String receivedData = "";

void setup() {
    Serial.begin(115200);
    Serial.println("HX8357D Test!");

    // Initialize ScreenManager
    screenManager.begin();

    uart1.init(115200);
}

void loop() {
    static unsigned long lastFrameTime = 0;
    const int frameDelay = 1000 / 11; // ~91ms per frame
    unsigned long currentTime = millis();

 
    // uint8_t* uartData = uart1.receive();
    //   Serial.println("Received Uart1 Driver: ");
    //   Serial.print((char*)uartData);

    // Read UART data
    uint8_t* uartData = uart1.receive();
    // Serial.println("Received from UART: ");
    // Serial.print(String((char*)uartData));  

    // Display UART data on the screen if something was received
    size_t len = strlen((char*)uartData);
    if (uartData != nullptr && len > 0) {
        Serial.print("Received from UART: ");
        Serial.println((char*)uartData);
        screenManager.displayUartData(uartData, len);
    }
    

    // from serial monitor
    // while(Serial.available()) {
    //     char incomingChar = Serial.read();

    //     if (incomingChar == '\n') {
    //         Serial.print("Received: ");
    //         Serial.println(receivedData);

    //         receivedData = "";
    //     } else {
    //         receivedData += incomingChar;
    //     }
    // }

   // Update screen only if frame delay time has passed
    if (currentTime - lastFrameTime >= frameDelay) {
        lastFrameTime = currentTime;
        screenManager.displayGrid();
    }



}


// testing drivers usage

// #include <Arduino.h>
// #include <Adafruit_GFX.h>    // Core graphics library
// #include <Adafruit_TFTLCD.h> // Hardware-specific library
// #include "src/spi1_driver.h"
// #include "src/spi2_driver.h"
// #include "src/esp_now_manager.h"
// #include "src/uart1_driver.h"

// SPI1Driver driver1;
// SPI2Driver driver2;
// UART1Driver uart1;
// ESPNowManager espNow;

// const char* ssid = "eduroam";
// const char* username = "cwu81";
// const char* password = "bJjtKG$k5OGtPY8Q";

// uint8_t receiverAddress[] = {0xF0, 0x9E, 0x9E, 0x1E, 0x49, 0xD8}; // Replace with actual MAC address

// // Nathan's board MAC address:
// // DC:DA:0C:2A:19:28

// // Christopher's board MAC address:
// // F0:9E:9E:21:EC:8C

// // Hector's board MAC address:
// // Mac Address: F0:9E:9E:1E:49:D8



// typedef struct example_message {
//   int data;
// } example_message;

// example_message myData;

// void setup() {
//     Serial.begin(115200);
//     // driver1.init();
//     // driver2.init();
    // espNow.init(ssid, username, password);
//     espNow.init_send(receiverAddress);
//     espNow.init_recv();
//     Serial.println("Mac Address: ");
//     Serial.println(WiFi.macAddress());
//     espNow.readMACAddress();
//     // uart1.init(115200);

   

// }

// void loop() {
//    myData.data = random(0x0000, 0xFFFF);


//   espNow.sendData(receiverAddress, (uint8_t*)&myData, sizeof(myData));
//   // uint8_t sample[] = {0x01, 0x02, 0x03, 0x04};
//   // uart1.send(sample, sizeof(sample));

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


// espnow send 


// #include <Arduino.h>
// // #include <Adafruit_GFX.h>    // Core graphics library
// // #include <Adafruit_TFTLCD.h> // Hardware-specific library
// #include "src/esp_now_manager.h"

// // Create ESPNowManager object
// ESPNowManager espNowManager;

// const char* ssid = "eduroam";
// const char* username = "cwu81";
// const char* password = "bJjtKG$k5OGtPY8Q";

// // Receiver's MAC Address (Hector)
// uint8_t receiverMAC[] = { 0xF0, 0x9E, 0x9E, 0x1E, 0x49, 0xD8 };

// // Structure for sending data
// typedef struct {
//   char port0[5]; // Only sending Port 0 data
// } DataPacket;

// DataPacket dataToSend;

// // Function to generate a random hex number for Port 0
// void generateHexForPort0() {
//   uint16_t hexValue = random(0x0000, 0xFFFF); // Generate new hex value
//   sprintf(dataToSend.port0, "%04X", hexValue); // Format as "XXXX"
// }

// void setup() {
//   Serial.begin(115200);
//   espNowManager.init(ssid, username, password);
//   // Print MAC Address of Sender
//   Serial.print("Sender MAC Address (Christopher): ");
//   espNowManager.readMACAddress();

//   // Initialize ESP-NOW for sending
//   espNowManager.init_send(receiverMAC);
// }

// void loop() {
//   // Generate a random number for Port 0
//   generateHexForPort0();

//   // Send data to receiver
//   int result = espNowManager.sendData(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));
  
//   if (result == 0) {
//     Serial.print("Sent Port 0 Data: ");
//     Serial.println(dataToSend.port0);
//   } else {
//     Serial.println("Error sending data");
//   }

//   delay(5000); // Send data every 100 ms (~10 FPS)
// }

// screen test


// #include <SPI.h>
// #include <Arduino.h>
// #include "Adafruit_GFX.h"
// #include "Adafruit_HX8357.h"
// #include "src/spi1_driver.h"
// #include "src/esp_now_manager.h"

// SPI1Driver driver1;
// ESPNowManager espNow;

// const char* ssid = "eduroam";
// const char* username = "cwu81";
// const char* password = "bJjtKG$k5OGtPY8Q";

// // Define SPI and TFT screen pins
// // #define TFT_SCK  39
// // #define TFT_MISO 38
// // #define TFT_MOSI 37
// #define TFT_CS   36
// #define TFT_DC   35
// #define TFT_RST  -1

// // Initialize display
// Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// // Store last hex values (4x4 grid)
// char lastHex[4][4][5];

// // Timing variables
// unsigned long lastFrameTime = 0;
// const int targetFPS = 11;
// const int frameDelay = 1000 / targetFPS;  // ~91ms per frame

// // Function to display static index numbers (drawn only once)
// void drawStaticIndexes() {
//   tft.setTextColor(HX8357_WHITE);
//   tft.setTextSize(2);

//   int startX = 10;
//   int startY = 30;
//   int rowSpacing = 40;
//   int colSpacing = 100;

//   for (int row = 0; row < 4; row++) {
//     for (int col = 0; col < 4; col++) {
//       int index = row + col * 4;
//       char indexStr[5];

//       sprintf(indexStr, "%2d:", index); // Format "0:", "4:", etc.

//       tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
//       tft.print(indexStr); // Print index once (no redraw needed)
//     }
//   }
// }

// // Function to erase only hex values (keeping index numbers)
// void eraseHexValues() {
//   tft.setTextColor(HX8357_BLACK); // Set text color to black
//   tft.setTextSize(2);

//   int startX = 50;  // Start after the index numbers
//   int startY = 30;
//   int rowSpacing = 40;
//   int colSpacing = 100;

//   for (int row = 0; row < 4; row++) {
//     for (int col = 0; col < 4; col++) {
//       tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
//       tft.print(lastHex[row][col]); // Overwrite old hex value
//     }
//   }
// }

// // Function to erase previous FPS (using a black rectangle)
// void erasePreviousFPS() {
//   tft.fillRect(10, 5, 120, 20, HX8357_BLACK); // Overwrite old FPS area
// }

// // Function to display FPS
// void displayFPS(float fps) {
//   erasePreviousFPS(); // Clear previous FPS correctly

//   char fpsStr[10];
//   sprintf(fpsStr, "FPS: %.1f", fps);

//   tft.setTextColor(HX8357_WHITE);
//   tft.setCursor(10, 5);
//   tft.print(fpsStr);
// }

// // Function to generate and display the 4x4 grid (only hex values)
// void displayGrid() {
//   unsigned long startTime = micros(); // Start frame timing

//   eraseHexValues(); // Only erase hex values (keep indexes)

//   tft.setTextColor(HX8357_WHITE);
//   tft.setTextSize(2);

//   int startX = 50;  // Start after the index numbers
//   int startY = 30;
//   int rowSpacing = 40;
//   int colSpacing = 100;

//   // Serial.println("\n=== Updated Hex Grid ===");

//   for (int row = 0; row < 4; row++) {
//     String rowOutput = "";

//     for (int col = 0; col < 4; col++) {
//       uint16_t hexValue = random(0x0000, 0xFFFF); // Generate new hex value
//       char hexStr[5];

//       sprintf(hexStr, "%04X", hexValue); // Format "XXXX"
//       strcpy(lastHex[row][col], hexStr); // Store new hex value

//       tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
//       tft.print(hexStr); // Print new hex value

//       rowOutput += String(hexStr) + "  ";
//     }
//     // Serial.println(rowOutput);
//   }
//   // Serial.println("==================================\n");

//   // Calculate FPS
//   unsigned long frameTime = micros() - startTime;
//   float fps = 1e6 / frameTime; // Convert microseconds to FPS

//   displayFPS(fps); // Update FPS display
// }

// void setup() {
//   Serial.begin(115200);
//   Serial.println("HX8357D Test!");
//   driver1.init();

//   tft.begin();
//   tft.setRotation(1);

//   // Fill screen once at startup (avoids unnecessary full redraws later)
//   tft.fillScreen(HX8357_BLACK);

//   // Initialize grid with blank spaces to prevent ghosting
//   for (int row = 0; row < 4; row++) {
//     for (int col = 0; col < 4; col++) {
//       strcpy(lastHex[row][col], "    "); // Empty spaces for clearing
//     }
//   }

//   // Draw the static indexes once
//   drawStaticIndexes();
//   displayGrid();

//   espNow.init(ssid, username, password);

//   Serial.println("Mac Address: ");
//   Serial.println(WiFi.macAddress());
  
// }

// void loop() {
//   unsigned long currentTime = millis();

//   // Update screen only if frame delay time has passed
//   if (currentTime - lastFrameTime >= frameDelay) {
//     lastFrameTime = currentTime;
//     displayGrid();
//   }
// }
