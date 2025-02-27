#include <SPI.h>
#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "src/spi1_driver.h"
#include "src/esp_now_manager.h"

// Create driver and ESPNowManager objects
SPI1Driver driver1;
ESPNowManager espNow;

const char* ssid = "eduroam";
const char* username = "cwu81";
const char* password = "bJjtKG$k5OGtPY8Q";

// Define SPI and TFT screen pins
#define TFT_CS   36
#define TFT_DC   35
#define TFT_RST  -1

// Structure for receiving data
typedef struct {
  char port1[5]; // Only receiving Port 1 data
} DataPacket;

DataPacket receivedData;

// Initialize display
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

char lastHex[4][4][5];
// Timing variables
unsigned long lastFrameTime = 0;
const int targetFPS = 11;
const int frameDelay = 1000 / targetFPS;  // ~91ms per frame
// Function to display static index numbers (drawn only once)
void drawStaticIndexes() {
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(2);
  int startX = 10;
  int startY = 30;
  int rowSpacing = 40;
  int colSpacing = 100;
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      int index = row + col * 4;
      char indexStr[5];
      sprintf(indexStr, "%2d:", index); // Format "0:", "4:", etc.
      tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
      tft.print(indexStr); // Print index once (no redraw needed)
    }
  }
    
  Serial.println("drawStaticIndexes Done"); 
}

// Function to erase only hex values (keeping index numbers)
void eraseHexValues() {
  tft.setTextColor(HX8357_BLACK); // Set text color to black
  tft.setTextSize(2);
  int startX = 50;  // Start after the index numbers
  int startY = 30;
  int rowSpacing = 40;
  int colSpacing = 100;
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
      tft.print(lastHex[row][col]); // Overwrite old hex value
    }
  }
}

// Function to erase previous FPS (using a black rectangle)
void erasePreviousFPS() {
  tft.fillRect(10, 5, 120, 20, HX8357_BLACK); // Overwrite old FPS area
}

// Function to display FPS
void displayFPS(float fps) {
  erasePreviousFPS(); // Clear previous FPS correctly
  char fpsStr[10];
  sprintf(fpsStr, "FPS: %.1f", fps);
  tft.setTextColor(HX8357_WHITE);
  tft.setCursor(10, 5);
  tft.print(fpsStr);
}

// Function to generate and display the 4x4 grid (only hex values)
void displayGrid() {
  unsigned long startTime = micros(); // Start frame timing
  eraseHexValues(); // Only erase hex values (keep indexes)
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(2);
  int startX = 50;  // Start after the index numbers
  int startY = 30;
  int rowSpacing = 40;
  int colSpacing = 100;
  // Serial.println("\n=== Updated Hex Grid ===");
  for (int row = 0; row < 4; row++) {
    String rowOutput = "";
    for (int col = 0; col < 4; col++) {
        uint16_t hexValue; // Generate new hex value

      if(row == 0 && col == 0){
        Serial.println("\n--- Port 1 Update ---");
        Serial.print("Received Data (String): ");
        Serial.println(receivedData.port1);

        // Convert received string to uint16_t
        hexValue = (uint16_t) strtol(receivedData.port1, NULL, 16);
        Serial.print("Converted to Hex: ");
        Serial.println(hexValue, HEX);
      } else{
        hexValue = random(0x0000, 0xFFFF); 
      }

      char hexStr[5]; 
      sprintf(hexStr, "%04X", hexValue); // Format "XXXX"
      strcpy(lastHex[row][col], hexStr); // Store new hex value
      tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
      tft.print(hexStr); // Print new hex value
      rowOutput += String(hexStr) + "  ";
    }
    // Serial.println(rowOutput);
  }
  // Serial.println("==================================\n");
  // Calculate FPS

  WiFi.mode(WIFI_STA); 
  espNow.init(ssid, username, password); 

  unsigned long frameTime = micros() - startTime;
  float fps = 1e6 / frameTime; // Convert microseconds to FPS
  displayFPS(fps); // Update FPS display
}


// ESP-NOW Callback for receiving data
void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // ** Print Received Data for Confirmation **
  Serial.print("Received Port 1 Data: ");
  Serial.println(receivedData.port1);
}

void setup() {
  Serial.begin(115200);
  // Initialize SPI and Display
  driver1.init();
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(HX8357_BLACK);

  // Initialize grid with blank spaces
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      strcpy(lastHex[row][col], "    "); // Empty spaces for clearing
    }
  }

  // Draw the static indexes once
  drawStaticIndexes();

  // Initialize ESP-NOW for receiving
  espNow.init(ssid, username, password);
  // esp_now_register_recv_cb(onDataReceived);

  Serial.println("Receiver Initialized!");
  Serial.println("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println("Setup Done"); 
}

void loop() {
  unsigned long currentTime = millis();
  // Update screen only if frame delay time has passed
  if (currentTime - lastFrameTime >= frameDelay) {
    lastFrameTime = currentTime;
    displayGrid();
    Serial.println("displayGrid"); 
  }

}




// #include <Arduino.h>
// #include "src/spi1_driver.h"
// #include "src/spi2_driver.h"
// #include "src/esp_now_manager.h"
// #include "Adafruit_GFX.h"
// #include "Adafruit_HX8357.h"
// #include <esp_now.h>
// #include <WiFi.h>
// #include "esp_eap_client.h"
// #include <esp_wifi.h>


// SPI1Driver driver1;
// SPI2Driver driver2;
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

// // Initialize display with hardware SPI
// Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// // Create ESPNowManager object
// ESPNowManager espNowManager;

// // Store last hex values (4x4 grid)
// char lastHex[4][4][5]; 

// // Timing variables for FPS
// unsigned long lastFrameTime = 0;
// const int targetFPS = 30;
// const int frameDelay = 1000 / targetFPS;  // Target 30 FPS

// // Layout Constants
// const int startX = 50;
// const int startY = 30;
// const int rowSpacing = 40;
// const int colSpacing = 100;

// // Last FPS value for clearing
// char lastFPS[10];

// // Structure for receiving data
// typedef struct {
//   char port0[5]; // Only receiving Port 0 data
// } DataPacket;

// DataPacket receivedData;

// // Callback function for receiving data
// void onDataReceived(const uint8_t *macAddr, const uint8_t *incomingData, int len) {
//   memcpy(&receivedData, incomingData, sizeof(receivedData));
// }

// // Function to display static index numbers (drawn only once)
// void drawStaticIndexes() {
//   tft.setTextColor(HX8357_WHITE);
//   tft.setTextSize(2);

//   for (int row = 0; row < 4; row++) {
//     for (int col = 0; col < 4; col++) {
//       int index = row + col * 4; 
//       char indexStr[5];

//       sprintf(indexStr, "%2d:", index);
      
//       tft.setCursor(startX - 40 + col * colSpacing, startY + row * rowSpacing);
//       tft.print(indexStr);
//     }
//   }
// }

// // Function to update Port 0 only
// void updatePort0() {
//   tft.setTextColor(HX8357_WHITE);  
//   tft.setTextSize(2);

//   // Only update if value changed
//   if (strcmp(lastHex[0][0], receivedData.port0) != 0) {
//     // Clear old value
//     tft.setTextColor(HX8357_BLACK);
//     tft.setCursor(startX, startY);
//     tft.print(lastHex[0][0]); // Clear the old value

//     // Store and print new value
//     strcpy(lastHex[0][0], receivedData.port0);
//     tft.setTextColor(HX8357_WHITE);
//     tft.setCursor(startX, startY);
//     tft.print(lastHex[0][0]);
//   }
// }

// void setup() {
//   Serial.begin(115200);
  
//   driver1.init();

//   tft.begin(48000000); 
//   tft.setRotation(1);

//   tft.fillScreen(HX8357_BLACK);

//   for (int row = 0; row < 4; row++) {
//     for (int col = 0; col < 4; col++) {
//       strcpy(lastHex[row][col], "    ");
//     }
//   }

//   drawStaticIndexes();

//   tft.setTextColor(HX8357_WHITE);
//   tft.setTextSize(2);
//   tft.setCursor(10, 5);
//   tft.print("FPS:");

//   // WiFi.mode(WIFI_STA);
//   espNow.init(ssid, username, password);
//   // Serial.print("Receiver MAC Address (Hector): ");
//   // espNowManager.readMACAddress();

//   // Initialize ESP-NOW for receiving
//   // espNowManager.init_recv();
//     esp_now_register_recv_cb(esp_now_recv_cb_t(onDataReceived));
// }

// void loop() {
//   unsigned long currentTime = millis();
//   if (currentTime - lastFrameTime >= frameDelay) {
//     lastFrameTime = currentTime;
//     updatePort0();
//   }
// }



// // uint8_t receiverAddress[] = {0xF0, 0x9E, 0x9E, 0x21, 0xEC, 0x8C}; // Replace with actual MAC address


// // typedef struct example_message {
// //   int data;
// // } example_message;

// // example_message myData;

// // void setup() {
// //     Serial.begin(115200);
// //     driver1.init();
//     // driver2.init();
//     // espNow.init(ssid, username, password);
//     // Serial.println("Mac Address: ");
//     // Serial.println(WiFi.macAddress());

//     // myData.data = 10;

// // }

// // void loop() {
// //   // espNow.sendData(receiverAddress, (uint8_t*)&myData, sizeof(myData));

// // //     const char *message = "Hello";
// // //     uint8_t data[] = {0x01, 0x02, 0x03, 0x04};

// // //     driver1.send(message);
// // //     Serial.printf("Sent from SPI1: %s\n", message);

// // //     driver1.send(data, sizeof(data));
// // //     Serial.printf("Sent from SPI1: 0x%02x 0x%02x 0x%02x 0x%02x\n", data[0], data[1], data[2], data[3]);

// // // //    if (SPIDriverBase::interrupt_flag) {
// // // //        Serial.println("Interrupt occurred!");
// // // //        SPIDriverBase::interrupt_flag = false;
// // // //    }

// // //     driver2.send(message);
// // //     Serial.printf("Sent from SPI2: %s\n", message);

// // //     driver2.send(data, sizeof(data));
// // //     Serial.printf("Sent from SPI2: 0x%02x 0x%02x 0x%02x 0x%02x\n", data[0], data[1], data[2], data[3]);

// // //     delay(10);
// // }
