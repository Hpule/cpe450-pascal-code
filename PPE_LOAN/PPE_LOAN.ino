// -------------------------- TESTING LIBRARY WITH CUSTOM FIRMWARE --------------------------
// Compiles with Hello World

#include <Arduino.h>
#include "src/ScreenManager.h"
#include "src/uart1_driver.h"
#include <sbus.h>

// Pin definitions
#define TFT_CS   37
#define TFT_DC   35
#define TFT_RST  1
#define TFT_LITE 2
#define SBUS_RX_PIN 18 // Change this to an available GPIO pin

// Create objects
ScreenManager screenManager(TFT_CS, TFT_DC, TFT_RST, TFT_LITE);
UART1Driver uart1;
String receivedData = "";

// SBUS objects
bfs::SbusRx sbus_rx(&Serial2, SBUS_RX_PIN, -1, false); // Using external inverter
bfs::SbusData sbusData;

// Flag to track if we have SBUS data
bool haveSbusData = false;

void setup() {
    Serial.begin(115200);
    Serial.println("SBUS Test with Grid Integration");

    // Initialize screen
    screenManager.begin();
    
    // Initialize UART1
    uart1.init(115200);
    
    // Initialize SBUS on Serial2
    Serial2.begin(100000, SERIAL_8E2, SBUS_RX_PIN, -1);
    sbus_rx.Begin();
    
    Serial.println("SBUS initialized on pin " + String(SBUS_RX_PIN));
    Serial.println("Waiting for SBUS data...");
}

void loop() {
    static unsigned long lastFrameTime = 0;
    const int frameDelay = 1000 / 11; // ~91ms per frame
    unsigned long currentTime = millis();
    static unsigned long lastSbusUpdate = 0;
    const int sbusUpdateInterval = 100; // Update every 100ms
    
    // Read and process SBUS data
    if (currentTime - lastSbusUpdate >= sbusUpdateInterval) {
        lastSbusUpdate = currentTime;
        
        if (sbus_rx.Read()) {
            sbusData = sbus_rx.data();
            haveSbusData = true;
            
            // Print SBUS data to serial monitor in a 4x4 grid format
            Serial.println("\n=== SBUS Channel Values ===");
            
            // Row 1: Channels 1-4
            Serial.print("CH 1-4:   ");
            for (int i = 0; i < 4; i++) {
                Serial.print(sbusData.ch[i]);
                Serial.print("\t");
            }
            Serial.println();
            
            // Row 2: Channels 5-8
            Serial.print("CH 5-8:   ");
            for (int i = 4; i < 8; i++) {
                Serial.print(sbusData.ch[i]);
                Serial.print("\t");
            }
            Serial.println();
            
            // Row 3: Channels 9-12
            Serial.print("CH 9-12:  ");
            for (int i = 8; i < 12; i++) {
                Serial.print(sbusData.ch[i]);
                Serial.print("\t");
            }
            Serial.println();
            
            // Row 4: Channels 13-16
            Serial.print("CH 13-16: ");
            for (int i = 12; i < 16; i++) {
                Serial.print(sbusData.ch[i]);
                Serial.print("\t");
            }
            Serial.println();
            
            // Status flags
            Serial.print("Lost Frame: ");
            Serial.print(sbusData.lost_frame ? "YES" : "NO");
            Serial.print("\tFailsafe: ");
            Serial.println(sbusData.failsafe ? "ACTIVE" : "OFF");
            Serial.println("===========================");
            
            // Update the grid with SBUS values
            screenManager.displaySbusGrid(sbusData.ch);
        }
    }
    
    // Update screen with normal grid if no SBUS data yet
    if (!haveSbusData && currentTime - lastFrameTime >= frameDelay) {
        lastFrameTime = currentTime;
        screenManager.displayGrid(); // Use random values if no SBUS data yet
    }
    
    // UART processing
    uint8_t* uartData = uart1.receive();
    size_t len = strlen((char*)uartData);
    if (uartData != nullptr && len > 0) {
        Serial.print("Received from UART: ");
        Serial.println((char*)uartData);
        screenManager.displayUartData(uartData, len);
    }
    
    // Rest of your code for LED and serial monitor
    pinMode(42, OUTPUT);
    digitalWrite(42, HIGH);
    delay(500);
    digitalWrite(42, LOW);
    delay(500);
    
    while(Serial.available()) {
        char incomingChar = Serial.read();
        if (incomingChar == '\n') {
            Serial.print("Received: ");
            Serial.println(receivedData);
            receivedData = "";
        } else {
            receivedData += incomingChar;
        }
    }
}

// #include <Arduino.h>
// #include "src/ScreenManager.h"
// #include "src/uart1_driver.h"
// #include <sbus.h>

// // Pin definitions
// #define TFT_CS   37
// #define TFT_DC   35
// #define TFT_RST  1
// #define TFT_LITE 2
// #define SBUS_RX_PIN 4 // Change this to an available GPIO pin

// // Create objects
// ScreenManager screenManager(TFT_CS, TFT_DC, TFT_RST, TFT_LITE);
// UART1Driver uart1;
// String receivedData = "";

// // SBUS objects
// bfs::SbusRx sbus_rx(&Serial2, SBUS_RX_PIN, -1, false); // Using external inverter
// bfs::SbusData sbusData;

// void setup() {
//     Serial.begin(115200);
//     Serial.println("SBUS Test - Terminal Output Only");

//     // Initialize screen
//     screenManager.begin();
    
//     // Initialize UART1
//     uart1.init(115200);
    
//     // Initialize SBUS on Serial2
//     Serial2.begin(100000, SERIAL_8E2, SBUS_RX_PIN, -1);
//     sbus_rx.Begin();
    
//     Serial.println("SBUS initialized on pin " + String(SBUS_RX_PIN));
//     Serial.println("Waiting for SBUS data...");
// }

// void loop() {
//     static unsigned long lastFrameTime = 0;
//     const int frameDelay = 1000 / 11; // ~91ms per frame
//     unsigned long currentTime = millis();
//     static unsigned long lastSbusUpdate = 0;
//     const int sbusUpdateInterval = 100; // Update every 100ms to avoid flooding serial

//     // Continue with your original screen update code
//     if (currentTime - lastFrameTime >= frameDelay) {
//         lastFrameTime = currentTime;
//         screenManager.displayGrid();
//     }
    
//     // Read and process SBUS data at a lower frequency
//     if (currentTime - lastSbusUpdate >= sbusUpdateInterval) {
//         lastSbusUpdate = currentTime;
        
//         if (sbus_rx.Read()) {
//             sbusData = sbus_rx.data();
            
//             // Print SBUS data to serial monitor in a 4x4 grid format
//             Serial.println("\n=== SBUS Channel Values ===");
            
//             // Row 1: Channels 1-4
//             Serial.print("CH 1-4:   ");
//             for (int i = 0; i < 4; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();
            
//             // Row 2: Channels 5-8
//             Serial.print("CH 5-8:   ");
//             for (int i = 4; i < 8; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();
            
//             // Row 3: Channels 9-12
//             Serial.print("CH 9-12:  ");
//             for (int i = 8; i < 12; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();
            
//             // Row 4: Channels 13-16
//             Serial.print("CH 13-16: ");
//             for (int i = 12; i < 16; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();
            
//             // Status flags
//             Serial.print("Lost Frame: ");
//             Serial.print(sbusData.lost_frame ? "YES" : "NO");
//             Serial.print("\tFailsafe: ");
//             Serial.println(sbusData.failsafe ? "ACTIVE" : "OFF");
//             Serial.println("===========================");
//         }
//     }
// }


// -------------------------- DRIVER TESTING??? --------------------------

// #include <Arduino.h>
// #include "src/i2c_driver.h"
// #include "src/spi1_driver.h"
// #include "src/test_functions.h"

// SPI1Driver spidriver1;
// I2CDriver i2cdriver;

// void setup() {
//   Serial.begin(115200);
//   spidriver1.init(1000000);
// }

// void loop() {
//  spidriver1.test_spi_pins(38, 37, 39, 36);
// //  i2cdriver.test_i2c_pins(9, 8, 100000, 0x28);
//  test_pin_blink(1, 1000);
// }

// -------------------------- DECONDING S8R SIGNAL USING HARDWARE INVERTER --------------------------

// #include <Arduino.h>
// #include "src/ScreenManager.h"
// #include "src/uart1_driver.h"
// #include "sbus.h"

// /* SBUS object, reading SBUS */
// bfs::SbusRx sbus_rx(&Serial2);  // ESP32 S3 has multiple serial ports, so we use Serial2

// /* SBUS object, writing SBUS */
// bfs::SbusTx sbus_tx(&Serial2);  // Only needed if you plan to send SBUS signals

// /* SBUS data */
// bfs::SbusData data;

// void setup() {
//   /* Serial to display data */
//   Serial.begin(115200);
  
//   /* For ESP32 S3, we need to explicitly set the pins for Serial2 */
//   Serial2.begin(100000, SERIAL_8E2, 16, 17);  // Adjust pins 16,17 to match your RX/TX pins
  
//   /* Begin the SBUS communication */
//   sbus_rx.Begin();
  
//   /* Only needed if sending SBUS signals */
//   sbus_tx.Begin();
  
//   Serial.println("ESP32 S3 SBUS Reader Started");
// }

// void loop() {
//   if (sbus_rx.Read()) {
//     /* Grab the received data */
//     data = sbus_rx.data();
    
//     /* Display the received data */
//     Serial.print("Channels: ");
//     for (int8_t i = 0; i < data.NUM_CH; i++) {
//       Serial.print(data.ch[i]);
//       Serial.print("\t");
//     }
    
//     /* Display lost frames and failsafe data */
//     Serial.print("Lost Frame: ");
//     Serial.print(data.lost_frame);
//     Serial.print("\t");
//     Serial.print("Failsafe: ");
//     Serial.println(data.failsafe);
    
//     /* Only needed if sending SBUS signals */
//     sbus_tx.data(data);
//     sbus_tx.Write();
//   }
// }





// -------------------------- TURN OFF BACKLIGHT DISPLAY --------------------------
// #include <Arduino.h>
// #include "src/ScreenManager.h"
// #include "src/uart1_driver.h"

// // Pin definitions
// #define TFT_CS   37
// #define TFT_DC   35
// #define TFT_RST  1
// #define TFT_LITE 2

// // Create ScreenManager object
// ScreenManager screenManager(TFT_CS, TFT_DC, TFT_RST, TFT_LITE);
// UART1Driver uart1;
// String receivedData = "";

// // Backlight state
// bool backlightOn = true;

// void setup() {
//     Serial.begin(115200);
//     Serial.println("HX8357D Test with Basic Backlight Control!");

//     // Set up the backlight pin as output
//     pinMode(TFT_LITE, OUTPUT);
    
//     // Turn on backlight initially
//     digitalWrite(TFT_LITE, HIGH);
//     Serial.println("Backlight is ON - send 'b' to toggle");

//     // Initialize ScreenManager
//     screenManager.begin();

//     uart1.init(115200);
// }

// void loop() {
//     static unsigned long lastFrameTime = 0;
//     const int frameDelay = 1000 / 11; // ~91ms per frame
//     unsigned long currentTime = millis();
 
//     // Read UART data
//     uint8_t* uartData = uart1.receive();
    
//     // Display UART data on the screen if something was received
//     size_t len = strlen((char*)uartData);
//     if (uartData != nullptr && len > 0) {
//         Serial.print("Received from UART: ");
//         Serial.println((char*)uartData);
//         screenManager.displayUartData(uartData, len);
//     }

//     pinMode(42, OUTPUT);
//     digitalWrite(42, HIGH);
//     delay(500);
//     digitalWrite(42, LOW);
//     delay(500);

//     // from serial monitor
//     while(Serial.available()) {
//         char incomingChar = Serial.read();

//         if (incomingChar == 'b') {
//             // Toggle backlight state
//             backlightOn = !backlightOn;
//             digitalWrite(TFT_LITE, backlightOn ? HIGH : LOW);
//             Serial.print("Backlight: ");
//             Serial.println(backlightOn ? "ON" : "OFF");
//         }
//         else if (incomingChar == '\n') {
//             Serial.print("Received: ");
//             Serial.println(receivedData);
//             receivedData = "";
//         } else {
//             receivedData += incomingChar;
//         }
//     }

//     // Update screen only if frame delay time has passed
//     if (currentTime - lastFrameTime >= frameDelay) {
//         lastFrameTime = currentTime;
//         screenManager.displayGrid();
//     }
// }

// -------------------------- DEMO DISPLAY --------------------------

// #include <Arduino.h>
// #include "src/ScreenManager.h"
// #include "src/uart1_driver.h"

// // Pin definitions
// #define TFT_CS   37
// #define TFT_DC   35
// #define TFT_RST  1
// #define TFT_LITE 2

// // Create ScreenManager object
// // screen manager initializes the spi1 driver already
// ScreenManager screenManager(TFT_CS, TFT_DC, TFT_RST, TFT_LITE);
// UART1Driver uart1;
// String receivedData = "";

// void setup() {
//     Serial.begin(115200);
//     Serial.println("HX8357D Test!");

//     // Initialize ScreenManager
//     screenManager.begin();

//     uart1.init(115200);
// }

// void loop() {
//     static unsigned long lastFrameTime = 0;
//     const int frameDelay = 1000 / 11; // ~91ms per frame
//     unsigned long currentTime = millis();

 
//     // uint8_t* uartData = uart1.receive();
//     //   Serial.println("Received Uart1 Driver: ");
//     //   Serial.print((char*)uartData);

//     // Read UART data
//     uint8_t* uartData = uart1.receive();
//     Serial.println("Received from UART: ");
//     Serial.print(String((char*)uartData));  

//     // Display UART data on the screen if something was received
//     size_t len = strlen((char*)uartData);
//     if (uartData != nullptr && len > 0) {
//         Serial.print("Received from UART: ");
//         Serial.println((char*)uartData);
//         screenManager.displayUartData(uartData, len);
//     }

//     pinMode(42, OUTPUT);
//     digitalWrite(42, HIGH);
//     delay(1000);
//     digitalWrite(42, LOW);
//     delay(1000);

    

//     // from serial monitor
//     while(Serial.available()) {
//         char incomingChar = Serial.read();

//         if (incomingChar == '\n') {
//             Serial.print("Received: ");
//             Serial.println(receivedData);

//             receivedData = "";
//         } else {
//             receivedData += incomingChar;
//         }
//     }

//   //  Update screen only if frame delay time has passed
//     if (currentTime - lastFrameTime >= frameDelay) {
//         lastFrameTime = currentTime;
//         screenManager.displayGrid();
//     }
// }
// ---------------------------------------------------------------------


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
