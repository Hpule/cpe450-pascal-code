#include "i2c_driver.h"

I2CDriver::I2CDriver() {
    // Optionally, set custom SDA/SCL pin numbers for your hardware configuration.
    SDA = 9;
    SCL = 8;
}

void I2CDriver::init() {
    // Begin I2C communication on the specified pins.
    Wire.begin(SDA, SCL);
    // Set the I2C clock speed.
    Wire.setClock(clockSpeed);
}

void I2CDriver::test_i2c_pins(int sda, int scl, int clockSpeed, int address) {
    Serial.println("Starting I2C test");
    Wire.begin(sda, scl);  // Initialize Wire library with custom SDA and SCL pins
    Wire.setClock(clockSpeed);  // Set custom clock speed

    uint8_t random_byte = random(0, 256);  // Generate random byte
    Wire.beginTransmission(address);  // Begin transmission to the specified address

    Wire.write(random_byte);  // Write the random byte to the I2C bus
    // byte result = Wire.endTransmission();  // End transmission and get the result
    uint8_t result = Wire.endTransmission();  // End transmission and get the result

    Serial.print("Sent: ");
    Serial.println(random_byte);
    Serial.print("Result: ");
    Serial.println(result);

    if (result == 0) {
        Serial.println("I2C test passed: Device acknowledged the message!");
    } else {
        Serial.println("I2C test failed: Device did not acknowledge the message!");
    }

    delay(1000);
}