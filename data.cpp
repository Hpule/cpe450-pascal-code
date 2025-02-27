#include <cstdint>
#include <cstdlib>
#include <cstdio>

class data {
    private:
        uint8_t *spi2_data;    
        uint8_t *spi1_data;
        uint8_t *i2c_data;
        uint8_t *motor_data;
        uint8_t *uart_data;
        uint8_t *io_data;
        uint8_t *speaker_data;
        uint8_t *rc_data;
    public:
        uint8_t *get_spi2_data();
        uint8_t *get_spi1_data();
        uint8_t *get_i2c_data();
        uint8_t *get_motor_data();
        uint8_t *get_uart_data();
        uint8_t *get_io_data();
        uint8_t *get_speaker_data();
        uint8_t *get_rc_data();
};