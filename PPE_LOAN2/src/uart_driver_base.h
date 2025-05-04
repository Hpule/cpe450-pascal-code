#ifndef UART_DRIVER_BASE_H
#define UART_DRIVER_BASE_H

#include <HardwareSerial.h>

class UARTDriverBase {
    public:
        UARTDriverBase();
        virtual ~UARTDriverBase();
        virtual void init(long baud_rate) = 0;
        virtual void send(const uint8_t* data, size_t len) = 0;
        virtual uint8_t* receive() = 0;

    protected:
        HardwareSerial* serial;
};

#endif // UART_DRIVER_BASE_H