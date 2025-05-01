#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h>

// Define constants
#define DOME_SERVO_PIN 46     // Change to your actual servo control pin
#define DOME_SBUS_CHANNEL 14  // SBUS channel for dome control (from Astro Mech code)

class ServoControl {
public:
    ServoControl();
    
    // Initialize the servo
    void begin();
    
    // Set servo angle (0-180)
    void setAngle(int angle);
    
    // Update servo based on SBUS data
    void updateFromSbus(const int16_t* channels);
    
private:
    Servo domeServo;
    int lastPosition;
};

#endif // SERVO_CONTROL_H   