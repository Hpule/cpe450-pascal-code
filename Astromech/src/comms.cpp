#include <cstdint>
#include <cstdlib>
#include <cstdio>

class comms {
    private:
        uint8_t *jetson_data;
        uint8_t *esp_now_data;
    public:
        void rc_receive(uint8_t *data);
        void connect_esp();
        void jetson_send(uint8_t *jetson_data);
};