#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <cstring>

class data {
    private:
        uint16_t spi2_data;    
        uint16_t spi1_data;
        uint16_t i2c_data;
        uint16_t motor_data;
        uint16_t uart_data;
        uint16_t io_data;
        uint16_t speaker_data;
        uint16_t rc_data;
    public:
        uint16_t get_spi2_data();
        uint16_t get_spi1_data();
        uint16_t get_i2c_data();
        uint16_t get_motor_data();
        uint16_t get_uart_data();
        uint16_t get_io_data();
        uint16_t get_speaker_data();
        uint16_t get_rc_data();
};

uint16_t get_rc_data() {
    // Replace this with your actual RC signal reading logic
    static uint16_t value = 1000;
    value += 1000; // Simulate changing values
    return value;
}

void store_rc_data() {
    // Declare a 4x4 array to store hex strings
    std::array<std::array<std::string, 4>, 4> lastHex;

    // Nested loop to populate the array
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            // Get the RC signal value
            uint16_t rcValue = get_rc_data();

            // Format the RC value as a 4-character hex string
            std::ostringstream oss;
            oss << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << rcValue;
            std::string rcStr = oss.str();

            // Store the RC value in the array
            lastHex[row][col] = rcStr;
        }
    }

    // Print the array contents to verify
    std::cout << "=== Stored RC Values ===" << std::endl;
    for (const auto & row : lastHex) {
        for (const auto& value : row) {
            std::cout << value << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << "==================================" << std::endl;
}
