class drivers {
    public:
        void motor_control();
        void i2c_transmit();
        void spi_1_transmit();
        void spi_2_transmit();
        void uart_transmit();
        void speaker_transmit();
        void init();
        void io_transmit();
};