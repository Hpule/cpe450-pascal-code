#include "ScreenManager.h"

SPI1Driver spi1;

ScreenManager::ScreenManager(int tft_cs, int tft_dc, int tft_rst)
: tft(tft_cs, tft_dc, tft_rst),

TFT_CS(tft_cs), TFT_DC(tft_dc), TFT_RST(tft_rst) {}

void ScreenManager::begin() {
    spi1.init(1000000);
    tft.begin();
    tft.setRotation(1);

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
    tft.setTextSize(2);

    int startX = 10;
    int startY = 30;
    int rowSpacing = 40;
    int colSpacing = 100;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int index = row + col * 4;
            char indexStr[5];

            sprintf(indexStr, "%2d:", index); // Format " 0:" to "4:", etc.

            tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
            tft.print(indexStr);
        }
    }
}

void ScreenManager::eraseHexValues() {
    tft.setTextColor(HX8357_BLACK);
    tft.setTextSize(2);

    int startX = 50;
    int startY = 30;
    int rowSpacing = 40;
    int colSpacing = 100;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (row == 0 && col == 0) {
                continue; // Skip first 4 bytes
            }
            tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
            tft.print(lastHex[row][col]);
        }
    }
}

void ScreenManager::erasePreviousFPS() {
    tft.fillRect(10, 5, 100, 20, HX8357_BLACK); // Overwrite old FPS area
}

void ScreenManager::displayFPS(float fps) {
    erasePreviousFPS();

    char fpsStr[10];
    sprintf(fpsStr, "FPS: %.1f", fps);

    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(10, 5);
    tft.print(fpsStr);
}

void ScreenManager::displayGrid() {
    unsigned long startTime = micros();

    eraseHexValues();

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
            if (row == 0 && col == 0) {
                rowOutput += "    "; // Skip first 4 bytes
            } else {
                uint16_t hexValue = random(0x0000, 0xFFFF); // Generate new hex value
                char hexStr[5];

                sprintf(hexStr, "%04X", hexValue); // Format "XXXX"
                strcpy(lastHex[row][col], hexStr); // Store new hex value

                tft.setCursor(startX + col * colSpacing, startY + row * rowSpacing);
                tft.print(hexStr); // Print new hex value

                rowOutput += String(hexStr) + "  ";
            }
        }
        // Serial.println(rowOutput);
    }
    // Serial.println("==================================\n");

    // Calculate FPS
    unsigned long frameTime = micros() - startTime;
    float fps = 1e6 / frameTime; // Convert microseconds to FPS

    displayFPS(fps); // Update FPS display
}

void ScreenManager::displayUartData(uint8_t* data, size_t len) {
    char hexStr[5];
    tft.setTextColor(HX8357_BLACK);
    tft.setTextSize(2);
    tft.setCursor(50 + 0 * 100, 30 + 0 * 40);
    tft.print(lastHex[0][0]); // Erase previous hex value

    sprintf(hexStr, "%04X", *(uint16_t*)data); // Convert first 2 bytes to hex string
    strcpy(lastHex[0][0], hexStr); // Store new hex value
    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(50 + 0 * 100, 30 + 0 * 40);
    tft.print(hexStr); // Print new hex value
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