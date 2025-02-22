#include <Arduino.h>

// Task handles
TaskHandle_t Task1;
TaskHandle_t Task2;

// Task 1: Handle ESP-NOW communication
void espNowTask(void *pvParameters) {
    while (true) {
        // Simulate sending/receiving data
        Serial.println("ESP-NOW Task Running");
        delay(1000); // Simulate task delay
    }
}

// Task 2: Process data or perform logging
void processDataTask(void *pvParameters) {
    while (true) {
        Serial.println("Processing Data Task Running");
        delay(2000); // Simulate task delay
    }
}

void setup() {
    Serial.begin(115200);

    // Create tasks pinned to specific cores
    xTaskCreatePinnedToCore(
        espNowTask,       // Function to execute
        "ESP-NOW Task",   // Name of the task
        10000,            // Stack size in bytes
        NULL,             // Parameters passed to the task
        1,                // Priority (higher = more important)
        &Task1,           // Task handle
        0                 // Core 0
    );

    xTaskCreatePinnedToCore(
        processDataTask,
        "Process Data Task",
        10000,
        NULL,
        1,
        &Task2,
        1                 // Core 1
    );
}

void loop() {
    // The main loop can remain empty or handle lower-priority tasks
}
