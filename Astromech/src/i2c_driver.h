#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

<<<<<<< Updated upstream:Astromech/src/i2c_driver.h
=======
#include <Wire.h>
>>>>>>> Stashed changes:Astromech/Naboo_Networks/src/i2c_driver.h
#include "i2c_driver_base.h"

class I2CDriver : public I2CDriverBase {
public:
    I2CDriver();
    void init() override;
<<<<<<< Updated upstream:Astromech/src/i2c_driver.h
};

#endif // I2C_DRIVER_ESP32S3_H
=======
    void transmit(uint8_t address, const char *str);
};

#endif // I2C_DRIVER_H
>>>>>>> Stashed changes:Astromech/Naboo_Networks/src/i2c_driver.h
