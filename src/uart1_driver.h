#ifndef UART1_DRIVER_H
#define UART1_DRIVER_H

#include "uart_driver_base.h"

class UART1Driver : public UARTDriverBase {
    public:
        UART1Driver();
        void init(long baud_rate) override;
        void send(const uint8_t* data, size_t len) override;
        uint8_t* receive() override;
};

#endif // UART1_DRIVER_H