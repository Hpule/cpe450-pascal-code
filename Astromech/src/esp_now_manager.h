#ifndef ESP_NOW_MANAGER_H
#define ESP_NOW_MANAGER_H

#include <esp_now.h>
#include <WiFi.h>
#include "esp_eap_client.h"

// class to manage ESP-NOW communication
class ESPNowManager {
public:
    ESPNowManager();
    void init(const char* ssid, const char* username, const char* password);
    esp_now_peer_info_t peerInfo;
    int sendData(const uint8_t* receiverAddress, const uint8_t* data, size_t len);

    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

private:
    static int lastSendSuccess;
    // for wpa2 enterprise
    uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    void setupWifi(const char* ssid, const char* username, const char* password);
};

#endif // ESP_NOW_MANAGER_H