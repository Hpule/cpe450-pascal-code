#include "esp_now_manager.h"

// static variables
int ESPNowManager::lastSendSuccess = 0;

ESPNowManager::ESPNowManager() {}

// initialize ESP-NOW
void ESPNowManager::init(const char* ssid, const char* username, const char* password) {

    // setup WiFi with WPA2 Enterprise
    setupWifi(ssid, username, password);

    // initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed");
        return;
    }

    uint8_t baseMac[6];

    // get MAC address of the device and form a unique peer address
    esp_now_register_send_cb(onDataSent);

    // register callback function for receiving data
    esp_now_register_recv_cb(esp_now_recv_cb_t(onDataRecv));
}

void ESPNowManager::setupWifi(const char* ssid, const char* username, const char* password) {
    // disconnect from WiFi
    WiFi.disconnect(true);

    // set WiFi mode to station
    WiFi.mode(WIFI_STA);

    // connect to WiFi
    WiFi.begin(ssid);

// function to set up wpa2 enterprise
esp_eap_client_set_identity((uint8_t *)username, strlen(username));

// function to setup wpa2 enterprise
esp_eap_client_set_username((uint8_t *)username, strlen(username));

// function to set up wpa2 enterprise
esp_eap_client_set_password((uint8_t *)password, strlen(password));

// enable wpa2 enterprise
esp_wifi_sta_enterprise_enable();

// while loop to wait for WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("");
    Serial.println("Connected to WiFi");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

int ESPNowManager::sendData(const uint8_t* receiverAddress, const uint8_t* data, size_t len) {
    // get MAC address of the device and form a unique peer address
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return -1;
    }

    // send data
    esp_err_t result = esp_now_send(receiverAddress, data, len);

    if (result != ESP_OK) {
        Serial.println("Failed to send data");
        return -1;
    }

    return 0;
}

// callback function for sending data
void ESPNowManager::onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
    lastSendSuccess = (status == ESP_NOW_SEND_SUCCESS) ? 1 : -1;
}

// callback function for receiving data
void ESPNowManager::onDataRecv(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
    Serial.println("Data received");
    Serial.print("MAC address: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac_addr[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println("");
    Serial.print("Data: ");
    for (int i = 0; i < data_len; i++) {
        Serial.printf("%02X", data[i]);
    }
    Serial.println("");
}