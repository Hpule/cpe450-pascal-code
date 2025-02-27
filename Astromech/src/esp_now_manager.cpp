// #include "esp_now_manager.h"

// // static variables
// int ESPNowManager::lastSendSuccess = 0;

// ESPNowManager::ESPNowManager() {}

// // initialize ESP-NOW
// void ESPNowManager::init(const char* ssid, const char* username, const char* password) {

<<<<<<< Updated upstream:Astromech/src/esp_now_manager.cpp
    // setup WiFi with WPA2 Enterprise
    setupWifi(ssid, esp_now_register_recv_cbusername, password);
=======
//     // setup WiFi with WPA2 Enterprise
//     setupWifi(ssid, esp_now_register_recv_cbusername, password);
>>>>>>> Stashed changes:Astromech/Naboo_Networks/src/esp_now_manager.cpp

//     // initialize ESP-NOW
//     if (esp_now_init() != ESP_OK) {
//         Serial.println("ESP-NOW initialization failed");
//         return;
//     }

<<<<<<< Updated upstream:Astromech/src/esp_now_manager.cpp
}

void ESPNowManager::init_send(const uint8_t* receiverAddress) {
    // get MAC address of the device and form a unique peer address
    esp_now_register_send_cb(onDataSent);

    // get MAC address of the device and form a unique peer address
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    Serial.println("Peer added");
}

void ESPNowManager::init_recv() {
    // register callback function for receiving data
    esp_now_register_recv_cb(esp_now_recv_cb_t(onDataRecv));
}
=======
// }

// void ESPNowManager::init_send(const uint8_t* receiverAddress) {
//     // get MAC address of the device and form a unique peer address
//     esp_now_register_send_cb(onDataSent);

//     // get MAC address of the device and form a unique peer address
//     esp_now_peer_info_t peerInfo;
//     memset(&peerInfo, 0, sizeof(peerInfo));
//     memcpy(peerInfo.peer_addr, receiverAddress, 6);
//     peerInfo.channel = 0;
//     peerInfo.encrypt = false;
>>>>>>> Stashed changes:Astromech/Naboo_Networks/src/esp_now_manager.cpp

//     // add peer
//     if (esp_now_add_peer(&peerInfo) != ESP_OK) {
//         Serial.println("Failed to add peer");
//         return;
//     }

//     Serial.println("Peer added");
// }

// void ESPNowManager::init_recv() {
//     // register callback function for receiving data
//     esp_now_register_recv_cb(esp_now_recv_cb_t(onDataRecv));
// }

<<<<<<< Updated upstream:Astromech/src/esp_now_manager.cpp
    if (username == nullptr || strlen(username) == 0) {
        // Regular Wi-Fi connection without WPA2 enterprise
        WiFi.begin(ssid, password);
        Serial.println("Connecting to WiFi...");

        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi...");
        }

        Serial.println("");
        Serial.println("Connected to WiFi");
    } else {
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

    }

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

int ESPNowManager::sendData(const uint8_t* receiverAddress, const uint8_t* data, size_t len) {
=======
// void ESPNowManager::setupWifi(const char* ssid, const char* username, const char* password) {
//     // disconnect from WiFi
//     WiFi.disconnect(true);

//     // set WiFi mode to station
//     WiFi.mode(WIFI_STA);

//     // connect to WiFi
//     WiFi.begin(ssid);

//     if (username == nullptr || strlen(username) == 0) {
//         // Regular Wi-Fi connection without WPA2 enterprise
//         WiFi.begin(ssid, password);
//         Serial.println("Connecting to WiFi...");

//         while (WiFi.status() != WL_CONNECTED) {
//             delay(1000);
//             Serial.println("Connecting to WiFi...");
//         }

//         Serial.println("");
//         Serial.println("Connected to WiFi");
//     } else {
//     // function to set up wpa2 enterprise
//     esp_eap_client_set_identity((uint8_t *)username, strlen(username));

//     // function to setup wpa2 enterprise
//     esp_eap_client_set_username((uint8_t *)username, strlen(username));
>>>>>>> Stashed changes:Astromech/Naboo_Networks/src/esp_now_manager.cpp

//     // function to set up wpa2 enterprise
//     esp_eap_client_set_password((uint8_t *)password, strlen(password));

//     // enable wpa2 enterprise
//     esp_wifi_sta_enterprise_enable();

//     // while loop to wait for WiFi connection
//         while (WiFi.status() != WL_CONNECTED) {
//             delay(1000);
//             Serial.println("Connecting to WiFi...");
//         }
//         Serial.println("");
//         Serial.println("Connected to WiFi");

//     }

<<<<<<< Updated upstream:Astromech/src/esp_now_manager.cpp
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

void ESPNowManager::readMACAddress() {
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}
=======
//     Serial.println("IP address: ");
//     Serial.println(WiFi.localIP());
// }

// int ESPNowManager::sendData(const uint8_t* receiverAddress, const uint8_t* data, size_t len) {

//     // send data
//     esp_err_t result = esp_now_send(receiverAddress, data, len);

//     if (result != ESP_OK) {
//         Serial.println("Failed to send data");
//         return -1;
//     }

//     return 0;
// }

// // callback function for sending data
// void ESPNowManager::onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
//     lastSendSuccess = (status == ESP_NOW_SEND_SUCCESS) ? 1 : -1;
// }

// // callback function for receiving data
// void ESPNowManager::onDataRecv(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
//     Serial.println("Data received");
//     Serial.print("MAC address: ");
//     for (int i = 0; i < 6; i++) {
//         Serial.printf("%02X", mac_addr[i]);
//         if (i < 5) Serial.print(":");
//     }
//     Serial.println("");
//     Serial.print("Data: ");
//     for (int i = 0; i < data_len; i++) {
//         Serial.printf("%02X", data[i]);
//     }
//     Serial.println("");
// }

// void ESPNowManager::readMACAddress() {
//   uint8_t baseMac[6];
//   esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
//   if (ret == ESP_OK) {
//     Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
//                   baseMac[0], baseMac[1], baseMac[2],
//                   baseMac[3], baseMac[4], baseMac[5]);
//   } else {
//     Serial.println("Failed to read MAC address");
//   }
// }
>>>>>>> Stashed changes:Astromech/Naboo_Networks/src/esp_now_manager.cpp
