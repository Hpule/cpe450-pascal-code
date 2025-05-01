#include "ServoControl.h"

ServoControl::ServoControl() {
    lastPosition = -1; // Initialize with invalid position to ensure first update
}

void ServoControl::begin() {
    // Allow allocation of timer
    ESP32PWM::allocateTimer(0);
    
    // Configure servo
    domeServo.setPeriodHertz(50);    // Standard 50Hz servo
    domeServo.attach(DOME_SERVO_PIN, 500, 2500); // Min/max pulse width
    
    // Center the servo initially
    setAngle(90);
    
    Serial.println("Servo control initialized");
}

void ServoControl::setAngle(int angle) {
    // Constrain angle to valid range
    int constrainedAngle = constrain(angle, 0, 180);
    
    // Only update if position has changed
    if (constrainedAngle != lastPosition) {
        domeServo.write(constrainedAngle);
        lastPosition = constrainedAngle;
        
        Serial.print("Servo set to angle: ");
        Serial.println(constrainedAngle);
    }
}

void ServoControl::updateFromSbus(const int16_t* channels) {
    // Get the raw SBUS value from the dome channel
    int16_t rawValue = channels[DOME_SBUS_CHANNEL];
    
    // If signal is too low, center the servo
    if (rawValue < 500) {
        setAngle(90);
        return;
    }
    
    // Process similar to Astro Mech approach:
    // 1. Subtract center value (approx 1000) to get a value centered around 0
    // 2. Scale down (similar to their bit-shift operation)
    // 3. Add to center position (90 degrees)
    
    int16_t adjustedValue = rawValue - 999;
    int16_t servoAngle = 90 + (adjustedValue / 16);
    
    // Set the servo angle
    setAngle(servoAngle);
}