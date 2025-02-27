#include <cstdint>
#include <cstdlib>
#include <cstdio>

class ui {
    private:
        uint8_t *screen;
        uint8_t *menu;
        uint8_t leds;
        uint8_t *dpad;
        uint8_t *buttons;
    public:
        uint8_t *display_menu(uint8_t *menu);
        void update_leds(uint8_t led_state);
        uint8_t *read_dpad(uint8_t *dpad);
        uint8_t *read_buttons(uint8_t *buttons);
};