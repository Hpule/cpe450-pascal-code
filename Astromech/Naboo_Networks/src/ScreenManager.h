#ifndef ScreenManager_h
#define ScreenManager_h

#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "spi1_driver.h"

class ScreenManager
{
public:
    ScreenManager(int tft_cs, int tft_dc, int tft_rst);

    void begin();

    // functions we want to add to display our own layout
    void displayGrid();
    void drawStaticIndexes();
    void eraseHexValues();
    void erasePreviousFPS();
    void displayFPS(float fps);
    void displayUartData(uint8_t* data, size_t len);

private:
    Adafruit_HX8357 tft;
    const int TFT_CS;
    const int TFT_DC;
    const int TFT_RST;

    char lastHex[4][4][5];

    unsigned long lastFrameTime = 0;
    const int targetFPS = 11;
    const int frameDelay = 1000 / targetFPS;
};

#endif // ScreenManager_h
