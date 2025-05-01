#include "ScreenManager.h"

SPI1Driver spi1;

ScreenManager::ScreenManager(int tft_cs, int tft_dc, int tft_rst, int tft_lite)
: tft(tft_cs, tft_dc, tft_rst),
  TFT_CS(tft_cs), TFT_DC(tft_dc), TFT_RST(tft_rst), TFT_LITE(tft_lite){}

void ScreenManager::begin() {
    spi1.init(1000000);
    tft.begin();
    
    // Set to portrait (0) or portrait flipped (2) for vertical orientation
    tft.setRotation(0); // 0 for normal portrait, 2 for flipped portrait
    
    tft.fillScreen(HX8357_BLACK);

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            strcpy(lastHex[row][col], "    "); // Empty spaces for clearing
        }
    }

    drawStaticIndexes();
    displayGrid();
}

void ScreenManager::drawStaticIndexes() {
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(TEXT_SIZE); // Keep at 2 to fit on screen

    // Adjusted spacing for portrait mode
    int startX = 10;
    int startY = 60; // Start lower to give room for FPS at top
    int rowSpacing = 35; // Slightly decreased to fit 4 rows
    int colSpacing = 80; // Decreased to fit 4 columns in portrait mode

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int index = row * 4 + col; // Changed index calculation for row-oriented grid
            char indexStr[5];

            sprintf(indexStr, "%2d:", index); // Format " 0:" to "15:"

            tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
            tft.print(indexStr);
        }
    }
}

void ScreenManager::eraseHexValues() {
    tft.setTextColor(HX8357_BLACK);
    tft.setTextSize(TEXT_SIZE);

    // Adjusted spacing to match drawStaticIndexes
    int startX = 45; // Decreased from 50
    int startY = 60; // Matches drawStaticIndexes
    int rowSpacing = 35; // Matches drawStaticIndexes
    int colSpacing = 80; // Matches drawStaticIndexes

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (row == 0 && col == 0) {
                continue; // Skip first position
            }
            tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
            tft.print(lastHex[row][col]);
        }
    }
}

void ScreenManager::erasePreviousFPS() {
    tft.fillRect(10, 20, 150, 30, HX8357_BLACK); // Positioned higher up
}

void ScreenManager::displayFPS(float fps) {
    erasePreviousFPS();

    char fpsStr[10];
    sprintf(fpsStr, "FPS: %.1f", fps);

    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(TEXT_SIZE);
    tft.setCursor(10, 20); // Positioned higher up
    tft.print(fpsStr);
}

void ScreenManager::displayGrid() {
    unsigned long startTime = micros();

    eraseHexValues();

    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(TEXT_SIZE);

    // Adjusted spacing to match drawStaticIndexes
    int startX = 45; // Decreased from 50
    int startY = 60; // Matches drawStaticIndexes
    int rowSpacing = 35; // Matches drawStaticIndexes
    int colSpacing = 80; // Matches drawStaticIndexes

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (row == 0 && col == 0) {
                continue; // Skip first position
            } else {
                uint16_t hexValue = random(0x0000, 0xFFFF); // Generate new hex value
                char hexStr[5];

                sprintf(hexStr, "%04X", hexValue); // Format "XXXX"
                strcpy(lastHex[row][col], hexStr); // Store new hex value

                tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
                tft.print(hexStr); // Print new hex value
            }
        }
    }

    // Calculate FPS
    unsigned long frameTime = micros() - startTime;
    float fps = 1e6 / frameTime; // Convert microseconds to FPS

    displayFPS(fps); // Update FPS display
}

void ScreenManager::displayUartData(uint8_t* data, size_t len) {
    char hexStr[5];
    tft.setTextColor(HX8357_BLACK);
    tft.setTextSize(TEXT_SIZE);
    tft.setCursor(45, 60); // Adjusted to match new grid
    tft.print(lastHex[0][0]); // Erase previous hex value

    sprintf(hexStr, "%04X", *(uint16_t*)data); // Convert first 2 bytes to hex string
    strcpy(lastHex[0][0], hexStr); // Store new hex value
    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(45, 60); // Adjusted to match new grid
    tft.print(hexStr); // Print new hex value
}

void ScreenManager::setHibernationMode(bool enable){
    if(enable){
        tft.sendCommand(HX8357_SLPIN); 
    } else {
        tft.sendCommand(HX8357_SLPOUT); 
    }
}

void ScreenManager::setLiteMode(bool enable, int tft_lite){
    // Fixed: replaced 'yn' with 'enable'
    analogWrite(tft_lite, enable ? 255 : 0);
}

void ScreenManager::displaySbusValues(int16_t* channels, bool lost_frame, bool failsafe) {
    // Clear a specific area for SBUS data
    tft.fillRect(0, 200, tft.width(), 120, HX8357_BLACK);
    
    // Display just the channel values without labels
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    
    int startX = 10;
    int startY = 210;
    int rowSpacing = 30;
    int colSpacing = 70;
    
    // Display all 16 channels in a 4x4 grid
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int index = row * 4 + col;
            tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
            tft.print(channels[index]);
        }
    }
    
    // Simple status indicators (just colored text)
    tft.setCursor(10, 330);
    tft.setTextColor(lost_frame ? HX8357_RED : HX8357_GREEN);
    tft.print(lost_frame ? "LOST" : "OK");
    
    tft.setCursor(120, 330);
    tft.setTextColor(failsafe ? HX8357_RED : HX8357_GREEN);
    tft.print(failsafe ? "FAIL" : "SAFE");
}

void ScreenManager::displaySbusGrid(int16_t* channels) {
    unsigned long startTime = micros();

    eraseHexValues();

    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(TEXT_SIZE);

    int startX = 45;
    int startY = 60;
    int rowSpacing = 35;
    int colSpacing = 80;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int index = row * 4 + col;
            if (row == 0 && col == 0) {
                continue; // Skip first position
            } else {
                char hexStr[5];
                sprintf(hexStr, "%04X", channels[index]); // Format channel as hex
                strcpy(lastHex[row][col], hexStr); // Store for future erasing

                tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
                tft.print(hexStr);
            }
        }
    }

    // Calculate FPS
    unsigned long frameTime = micros() - startTime;
    float fps = 1e6 / frameTime;

    displayFPS(fps);
}

// example usage

/*
// Pin definitions
#define TFT_CS   36
#define TFT_DC   35
#define TFT_RST  -1

// Create ScreenManager object
ScreenManager screenManager(TFT_CS, TFT_DC, TFT_RST);

void setup() {
    Serial.begin(115200);
    Serial.println("HX8357D Test!");

    // Initialize ScreenManager
    screenManager.begin();
}

void loop() {
    static unsigned long lastFrameTime = 0;
    const int frameDelay = 1000 / 11; // ~91ms per frame
    unsigned long currentTime = millis();

    // Update screen only if frame delay time has passed
    if (currentTime - lastFrameTime >= frameDelay) {
        lastFrameTime = currentTime;
        screenManager.displayGrid();
    }
}

*/