#ifndef ScreenManager_h
#define ScreenManager_h

#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "spi1_driver.h"

#define TEXT_SIZE 2
#define TEXT_ORIE 1
#define COL_SPACE 90
#define ROW_SPACE 45
#define X_START_DEL   45
#define Y_START   60 


class ScreenManager
{
public:
    ScreenManager(int tft_cs, int tft_dc, int tft_rst, int tft_lite);

    void begin();
    
    // Display SBUS values
    void displayGrid(int16_t* channels = nullptr, bool lost_frame = false, bool failsafe = false);
    void displayFPS(float fps);    

    // Shortcut to show disconnected state
    void displayDisconnected();
    
    // Utility functions
    void setHibernationMode(bool enable);
    void setLiteMode(bool enable, int tft_lite);

private:
    Adafruit_HX8357 tft;
    const int TFT_CS;
    const int TFT_DC;
    const int TFT_RST;
    const int TFT_LITE; 

    char lastHex[4][4][5];
    
    void drawStaticIndexes();
    void eraseHexValues();
    void erasePreviousFPS();

};

#endif // ScreenManager_h