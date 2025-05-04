#include "ScreenManager.h"

SPI1Driver spi1;

ScreenManager::ScreenManager(int tft_cs, int tft_dc, int tft_rst, int tft_lite)
: tft(tft_cs, tft_dc, tft_rst),
  TFT_CS(tft_cs), TFT_DC(tft_dc), TFT_RST(tft_rst), TFT_LITE(tft_lite){}

void ScreenManager::begin() {
    spi1.init(2000000);
    tft.begin();
    
    // Set to landscape orientation
    tft.setRotation(TEXT_ORIE);
    
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
    tft.setTextSize(TEXT_SIZE);

    int startX = 10;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int index = row * 4 + col;
            char indexStr[5];

            sprintf(indexStr, "%2d:", index); // Format " 0:" to "15:"

            tft.setCursor(startX + col * COL_SPACE, Y_START + row * ROW_SPACE);
            tft.print(indexStr);
        }
    }
}

void ScreenManager::eraseHexValues() {
    tft.setTextColor(HX8357_BLACK);
    tft.setTextSize(TEXT_SIZE);

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            tft.setCursor(X_START_DEL + col * COL_SPACE, Y_START + row * ROW_SPACE);
            tft.print(lastHex[row][col]);
        }
    }
}

void ScreenManager::erasePreviousFPS() {
    tft.fillRect(10, 20, 150, 30, HX8357_BLACK);
}

void ScreenManager::displayFPS(float fps) {
    erasePreviousFPS();

    char fpsStr[10];
    sprintf(fpsStr, "FPS: %.1f", fps);

    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(TEXT_SIZE);
    tft.setCursor(10, 20);
    tft.print(fpsStr);
}

void ScreenManager::displayGrid(int16_t* channels, bool lost_frame, bool failsafe) {
    // If no channels provided, display grid with zeros
    int16_t defaultChannels[16] = {0};
    if (channels == nullptr) {
        channels = defaultChannels;
    }
    
    // Main grid display for upper part of screen
    eraseHexValues();

    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(TEXT_SIZE);

    // Check if we're connected (any non-zero value)
    bool connected = false;
    for (int i = 0; i < 16; i++) {
        if (channels[i] != 0) {
            connected = true;
            break;
        }
    }

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int index = row * 4 + col;
            char hexStr[5];
            
            // Format as 4-digit decimal
            sprintf(hexStr, "%4d", channels[index]);
            strcpy(lastHex[row][col], hexStr); // Store for future erasing

            // Set color based on connection
            if (connected) {
                tft.setTextColor(HX8357_GREEN);
            } else {
                tft.setTextColor(HX8357_RED);
            }
            
            tft.setCursor(X_START_DEL + col * COL_SPACE, Y_START + row * ROW_SPACE);
            tft.print(hexStr);
        }
    }
    
    // Calculate appropriate position for status indicators based on grid size
    int statusY = Y_START + 4 * ROW_SPACE + 20; // Position below the grid with some margin
    
    // Display connection status
    tft.fillRect(0, statusY, tft.width(), 40, HX8357_BLACK);
    tft.setTextSize(TEXT_SIZE);
    
    // Connection status indicators
    tft.setCursor(10, statusY);
    if (connected) {
        tft.setTextColor(lost_frame ? HX8357_YELLOW : HX8357_GREEN);
        tft.print(lost_frame ? "LOST FRAME" : "CONNECTED");
    } else {
        tft.setTextColor(HX8357_RED);
        tft.print("DISCONNECTED");
    }
    
    // Calculate position for failsafe indicator
    int failsafeX = (TEXT_ORIE == 1) ? 180 : 10; // Different position based on orientation
    int failsafeY = (TEXT_ORIE == 1) ? statusY : statusY + 30; // Different row if portrait
    
    tft.setCursor(failsafeX, failsafeY);
    tft.setTextColor(failsafe ? HX8357_RED : HX8357_GREEN);
    tft.print(failsafe ? "FAILSAFE" : "NORMAL");
}

void ScreenManager::displayDisconnected() {
    // Create and use zero-filled array
    int16_t zeroChannels[16] = {0};
    displayGrid(zeroChannels, true, false);
}

void ScreenManager::setHibernationMode(bool enable){
    if(enable){
        tft.sendCommand(HX8357_SLPIN); 
    } else {
        tft.sendCommand(HX8357_SLPOUT); 
    }
}

void ScreenManager::setLiteMode(bool enable, int tft_lite){
    analogWrite(tft_lite, enable ? 255 : 0);
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