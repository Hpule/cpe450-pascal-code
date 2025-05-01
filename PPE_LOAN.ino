// -------------------------- Servo Testing + SBUS (NO Screen) --------------------------
#include <Arduino.h>

// Pin definitions for servos
#define H_SERVO_PIN 46  // Heading servo pin
#define L_SERVO_PIN 47  // Left servo pin
#define R_SERVO_PIN 48  // Right servo pin

// Menu options
#define TEST_H_SERVO 1
#define TEST_L_SERVO 2
#define TEST_R_SERVO 3
#define TEST_ALL_SERVOS 4
#define SIMULTANEOUS_MOVE 5
#define STOP_ALL_SERVOS 6


int selectedTest = 0;
bool servosActive = false;

void setup() {
  Serial.begin(115200);
  
  // Initialize servo pins
  pinMode(H_SERVO_PIN, OUTPUT);
  pinMode(L_SERVO_PIN, OUTPUT);
  pinMode(R_SERVO_PIN, OUTPUT);
  
  // Stop all servos initially
  stopAllServos();
  
  Serial.println("\n===== Simple Servo Test =====");
  Serial.println("1. Test Heading Servo (Pin 46)");
  Serial.println("2. Test Left Servo (Pin 47)");
  Serial.println("3. Test Right Servo (Pin 48)");
  Serial.println("4. Test All Servos Sequentially");
  Serial.println("5. Move All Servos Simultaneously");
  Serial.println("6. Stop All Servos");
  Serial.println("Enter selection (1-6):");
}

void sendServoPulse(int pin, int angle) {
  int pulseWidth = map(angle, 0, 180, 500, 2400);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(pin, LOW);
}

void testServoSweep(int pin) {
  String servoName;
  if (pin == H_SERVO_PIN) servoName = "Heading";
  else if (pin == L_SERVO_PIN) servoName = "Left";
  else if (pin == R_SERVO_PIN) servoName = "Right";
  
  Serial.println("Testing " + servoName + " servo on pin " + String(pin));
  Serial.println("Starting servo sweep test...");

  // Move to center position first
  Serial.println("Moving to center position");
  for(int i = 0; i < 20; i++) {  // Send multiple pulses to ensure it reaches position
    sendServoPulse(pin, 90);
    delay(20);
  }
  delay(1000);

  // Move to left position
  Serial.println("Moving to left position (0 degrees)");
  for(int angle = 90; angle >= 0; angle--) {
    sendServoPulse(pin, angle);
    delay(20);
  }
  delay(1000);

  // Sweep to right position
  Serial.println("Sweeping to right position (180 degrees)");
  for(int angle = 0; angle <= 180; angle++) {
    sendServoPulse(pin, angle);
    delay(20);
  }
  delay(1000);

  // Return to center
  Serial.println("Returning to center");
  for(int angle = 180; angle >= 90; angle--) {
    sendServoPulse(pin, angle);
    delay(20);
  }

  Serial.println("Test complete for " + servoName + " servo");
  Serial.println();
}


// Stop all servos by stopping signal generation
void stopAllServos() {
  // Move all servos to center position (90 degrees)
  for(int i = 0; i < 10; i++) {  // Send multiple pulses to ensure they reach position
    sendServoPulse(H_SERVO_PIN, 90);
    delay(5);
    sendServoPulse(L_SERVO_PIN, 90);
    delay(5);
    sendServoPulse(R_SERVO_PIN, 90);
    delay(5);
  }
  
  // Then stop sending pulses to reduce jitter and conserve power
  servosActive = false;
  Serial.println("All servos stopped and centered at 90 degrees");
}

void testAllServos() {
  Serial.println("Testing all servos sequentially");
  
  // Test each servo one at a time
  testServoSweep(H_SERVO_PIN);
  delay(1000);
  
  testServoSweep(L_SERVO_PIN);
  delay(1000);
  
  testServoSweep(R_SERVO_PIN);
    delay(1000);
  
  Serial.println("All servo tests complete");
}

// Send pulses to all servos simultaneously with the same angle
void moveAllServosTo(int angle) {
  // Calculate pulse width once for efficiency
  int pulseWidth = map(angle, 0, 180, 500, 2400);
  
  // Set all pins HIGH
  digitalWrite(H_SERVO_PIN, HIGH);
  digitalWrite(L_SERVO_PIN, HIGH);
  digitalWrite(R_SERVO_PIN, HIGH);
  
  // Wait for the pulse width duration
  delayMicroseconds(pulseWidth);
  
  // Set all pins LOW
  digitalWrite(H_SERVO_PIN, LOW);
  digitalWrite(L_SERVO_PIN, LOW);
  digitalWrite(R_SERVO_PIN, LOW);
}

// Test function to move all servos simultaneously
void testAllServosSynchronized() {
  Serial.println("Moving all servos simultaneously");
  
  // Center all servos first
  Serial.println("Moving to center position (90 degrees)");
  for(int i = 0; i < 20; i++) {
    moveAllServosTo(90);
    delay(20);
  }
  delay(1000);
  
  // Move to left position
  Serial.println("Moving to left position (0 degrees)");
  for(int angle = 90; angle >= 0; angle--) {
    moveAllServosTo(angle);
    delay(20);
  }
  delay(1000);
  
  // Sweep to right position
  Serial.println("Sweeping to right position (180 degrees)");
  for(int angle = 0; angle <= 180; angle++) {
    moveAllServosTo(angle);
    delay(20);
  }
  delay(1000);
  
  // Return to center
  Serial.println("Returning to center");
  for(int angle = 180; angle >= 90; angle--) {
    moveAllServosTo(angle);
    delay(20);
  }
  
  Serial.println("Synchronized movement test complete");
}

void loop() {
  // Check for user input to select test
  if (Serial.available() > 0) {
    selectedTest = Serial.parseInt();
    
    if (selectedTest >= 1 && selectedTest <= 6) {
      Serial.println("Selected option: " + String(selectedTest));
      servosActive = true;
      
      switch (selectedTest) {
        case TEST_H_SERVO:
          testServoSweep(H_SERVO_PIN);
          break;
        case TEST_L_SERVO:
          testServoSweep(L_SERVO_PIN);
          break;
        case TEST_R_SERVO:
          testServoSweep(R_SERVO_PIN);
          break;
        case TEST_ALL_SERVOS:
          testAllServos();
          break;
        case SIMULTANEOUS_MOVE:
          testAllServosSynchronized();
          break;
        case STOP_ALL_SERVOS:
          stopAllServos();
          break;
      }
      
      // Show menu again after test completes
      Serial.println("\n===== Simple Servo Test =====");
      Serial.println("1. Test Heading Servo (Pin 46)");
      Serial.println("2. Test Left Servo (Pin 47)");
      Serial.println("3. Test Right Servo (Pin 48)");
      Serial.println("4. Test All Servos Sequentially");
      Serial.println("5. Move All Servos Simultaneously");
      Serial.println("6. Stop All Servos");
      Serial.println("Enter selection (1-6):");
    }
    else {
      Serial.println("Invalid selection. Please enter 1-6.");
    }
    
    // Clear any remaining characters in the input buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
  
  // Small delay for stability
  delay(10);
}
// -------------------------- Servo Testing + SBUS (NO Screen) --------------------------
// #include <Arduino.h>
// #include <sbus.h>

// #define H_SERVO_PIN 46
// #define R_SERVO_PIN 48
// #define L_SERVO_PIN 47

// #define H_CHANNEL 15
// #define L_CHANNEL 6
// #define R_CHANNEL 7
//   // Change to your actual pin

// #define SERVO_UPDATE_MS 20  // Update interval in milliseconds
// #define SERVO_STEP 1        // Degrees to change per update

// #define SBUS_RX_PIN 18 // Change this to an available GPIO pin

// int currentAngle = 90;      // Starting angle
// int targetAngle = 90;       // Target angle
// unsigned long lastUpdate = 0;

// // SBUS objects
// bfs::SbusRx sbus_rx(&Serial2, SBUS_RX_PIN, -1, false); // Using external inverter
// bfs::SbusData sbusData;


// uint8_t servo_value = 127;

// // Min/Max Motor Speed for Debugger
// int max_motor_speed = 100;
// int min_motor_speed = -100;

// // Motor Speed Scalar
// int motor_speed_scalar = 0;

// // Bias for the Motor Speed
// int motor_speed_bias = 0;

// // Min/Max Servo Speed for Debugger
// int max_servo_speed = 100;
// int min_servo_speed = -100;

// void control_motors_joystick(uint16_t ver_val, uint16_t hor_val) {
//     // These calculations might need to be
//     // changed based on calibrations!!
//     // center both values at 0
//     int16_t hor_val_origin = hor_val - 124;
//     int16_t ver_val_origin = ver_val - 124;
//     int16_t left_motor = ver_val_origin + hor_val_origin;
//     int16_t right_motor = ver_val_origin - hor_val_origin;
//     // The Clamping of Values is Now Done in the Motor Servo Control
//     // ver_val represents throttle, hor_val represents steering

//     int16_t left_speed = ((left_motor * 100) >> 9);
//     int16_t right_speed = ((right_motor * 100) >> 9);

//     // Clamp Speed if Less Than 3
//     if (abs(left_speed) < 3)
//       left_speed = 0;
//     if (abs(right_speed) < 3)
//       right_speed = 0;

//     // SerialUSB.print(left_speed);
//     // SerialUSB.print(" ");
//     // SerialUSB.print(right_speed);
//     // SerialUSB.print("\n");

//     change_motor_speed(0, left_speed);
// }

// // Sets Motor Speed
// void change_motor_speed(uint8_t motor_num, int16_t speed) {

//     // Apply the Bias
//     if (abs(speed) > 5)
//         speed += motor_speed_bias;

//     // Left Motor
//     if (!motor_num) {
//         // Calculate speed value target
//         servo_value = transformSpeed(speed, min_motor_speed, max_motor_speed, motor_speed_scalar);
//     }

//     // Right Motor
//     else {
//         // Calculate speed value
//         servo_value = 255 - transformSpeed(speed, min_motor_speed, max_motor_speed, motor_speed_scalar);
//     }
// }


// uint8_t transformSpeed(int16_t speed, int min_speed, int max_speed, int scalar) {

//     // Apply the Scalar
//     if (speed > 0) {
//         speed += scalar;
//         if (speed < 0)
//             speed = 0;
//     }
//     else {
//         speed -= scalar;
//         if (speed > 0)
//             speed = 0;
//     }

//     // Clamp Speed Between Min and Max Speed
//     if (speed > max_speed)
//         speed = max_speed;
//     else if (speed < min_speed)
//         speed = min_speed;

//     // Transform Value into an Unsigned Byte
//     return 127 - speed;
// }

// void setup() {
//   Serial.begin(115200);
//   pinMode(H_SERVO_PIN OUTPUT);
//   pinMode(L_SERVO_PIN, OUTPUT);
//   pinMode(R_SERVO_PIN, OUTPUT);  

//   Serial.println("Servo gradual control test");
//   Serial.println("Commands: 'l' (left), 'c' (center), 'r' (right), '+'/'-' (adjust speed)");

//     // Initialize SBUS
//   Serial2.begin(100000, SERIAL_8E2, SBUS_RX_PIN, -1);
//   sbus_rx.Begin();
//   Serial.println("SBUS initialized on pin " + String(SBUS_RX_PIN));
// }

// void sendServoPulse(int angle) {
//   int pulseWidth = map(angle, 0, 180, 500, 2400);
//   digitalWrite(H_SERVO_PIN HIGH);
//   delayMicroseconds(pulseWidth);
//   digitalWrite(H_SERVO_PIN LOW);
// }



// void updateServo() {
//   unsigned long currentTime = millis();

//   // Send pulse regardless of angle change
//   sendServoPulse(currentAngle);

//   // Only update position at specified intervals
//   if (currentTime - lastUpdate >= SERVO_UPDATE_MS) {
//     lastUpdate = currentTime;

//     // Gradually move toward target
//     if (currentAngle < targetAngle) {
//       currentAngle += SERVO_STEP;
//     } else if (currentAngle > targetAngle) {
//       currentAngle -= SERVO_STEP;
//     }
//   }
// }


// void testServoSweep() {
//   Serial.println("Starting servo sweep test...");

//   // Move to left position
//   Serial.println("Moving to left position");
//   for(int angle = 90; angle >= 0; angle--) {
//     sendServoPulse(angle);
//     delay(20);
//   }
//   delay(1000);

//   // Sweep to right position
//   Serial.println("Sweeping to right position");
//   for(int angle = 0; angle <= 180; angle++) {
//     sendServoPulse(angle);
//     delay(20);
//   }
//   delay(1000);

//   // Return to center
//   Serial.println("Returning to center");
//   for(int angle = 180; angle >= 90; angle--) {
//     sendServoPulse(angle);
//     delay(20);
//   }

//   Serial.println("Test complete");
// }

// void loop() {
//   // Process commands
//   // if (Serial.available()) {
//   //   char cmd = Serial.read();
//   //   switch (cmd) {
//   //     case 'l': targetAngle = 0; break;    // Left
//   //     case 'c': targetAngle = 90; break;   // Center
//   //     case 'r': targetAngle = 180; break;  // Right
//   //     case '+': targetAngle += 10; break;  // Increment
//   //     case '-': targetAngle -= 10; break;  // Decrement
//   //   }
//   //   targetAngle = constrain(targetAngle, 0, 180);
//   //   Serial.print("Target angle: ");
//   //   Serial.println(targetAngle);
//   // }

//   // updateServo();
//   // delay(2); // Small delay for stability

//     if (sbus_rx.Read()) {
//         sbusData = sbus_rx.data();


//         Serial.println(sbusData.ch[14]);
//         uint16_t ver_8bit = sbusData.ch[14] >> 3;
//         uint16_t hor_8bit = ver_8bit;

//         control_motors_joystick(ver_8bit, hor_8bit);

//         Serial.println("Servo number: ");
//         Serial.print(servo_value);

//         targetAngle = constrain(servo_value, 0, 180);

//         updateServo();
//         delay(2);

//                 Serial.println(sbusData.ch[14]);
//         uint16_t ver_8bit = sbusData.ch[14] >> 3;
//         uint16_t hor_8bit = ver_8bit;

//         control_motors_joystick(ver_8bit, hor_8bit);

//         Serial.println("Servo number: ");
//         Serial.print(servo_value);

//         targetAngle = constrain(servo_value, 0, 180);

//         updateServo();
//         delay(2);
//         }

//   // testServoSweep();
// }

// -------------------------- SBUS Library + SCREEN +SERVO--------------------------

// #include <Arduino.h>
// #include "src/ScreenManager.h"
// #include "src/uart1_driver.h"
// #include "src/ServoControl.h"
// #include <sbus.h>

// // Pin definitions
// #define SBUS_RX_PIN 18  // Change to match your SBUS receiver pin

// // Create objects
// ServoControl servoControl;
// bfs::SbusRx sbus_rx(&Serial2, SBUS_RX_PIN, -1, false); // Using external inverter
// bfs::SbusData sbusData;

// // Test mode variables
// bool runTestMode = true;  // Set to false to disable test mode
// unsigned long testStartTime = 0;

// void setup() {
//   // Initialize serial communication
//   Serial.begin(115200);
//   Serial.println("ESP32 Dome Servo Test");

//   // Initialize servo control
//   servoControl.begin();

//   // Initialize SBUS
//   Serial2.begin(100000, SERIAL_8E2, SBUS_RX_PIN, -1);
//   sbus_rx.Begin();
//   Serial.println("SBUS initialized on pin " + String(SBUS_RX_PIN));

//   // Start test mode timer
//   testStartTime = millis();

//   // Instructions
//   Serial.println("----------------------------------------------------");
//   Serial.println("Test mode will run for 10 seconds, then switch to SBUS control");
//   Serial.println("To skip test mode, set runTestMode = false in the code");
//   Serial.println("----------------------------------------------------");
// }

// void loop() {
//   // Get current time
//   unsigned long currentTime = millis();

//   // Run test mode for first 10 seconds if enabled
//   if (runTestMode && (currentTime - testStartTime < 10000)) {
//     // Calculate test position based on time
//     // This creates a sweeping motion from 45 to 135 degrees and back
//     unsigned long cycleTime = (currentTime - testStartTime) % 4000;
//     int angle;

//     if (cycleTime < 2000) {
//       // Sweep from 45 to 135 degrees in 2 seconds
//       angle = map(cycleTime, 0, 2000, 45, 135);
//     } else {
//       // Sweep from 135 to 45 degrees in 2 seconds
//       angle = map(cycleTime, 2000, 4000, 135, 45);
//     }

//     // Set servo position
//     servoControl.setAngle(angle);

//     // Small delay to prevent overwhelming serial output
//     delay(50);
//   }
//   // Switch to SBUS control mode after test
//   else {
//     // Only run this once when transitioning from test mode
//     if (runTestMode) {
//       runTestMode = false;
//       Serial.println("----------------------------------------------------");
//       Serial.println("Test mode complete. Switching to SBUS control");
//       Serial.println("----------------------------------------------------");
//     }

//     // Process SBUS data
//     if (sbus_rx.Read()) {
//       sbusData = sbus_rx.data();

//       // Print SBUS channel data for dome control
//       Serial.print("Dome Channel (");
//       Serial.print(DOME_SBUS_CHANNEL);
//       Serial.print("): ");
//       Serial.println(sbusData.ch[DOME_SBUS_CHANNEL]);

//       // Update servo from SBUS data
//       servoControl.updateFromSbus(sbusData.ch);
//     }
//   }
// }

// -------------------------- Servo Testing --------------------------
// #include <Arduino.h>
// #include <sbus.h>

// #define SERVO_PIN 46
//   // Change to your actual pin

// #define SERVO_UPDATE_MS 20  // Update interval in milliseconds
// #define SERVO_STEP 1        // Degrees to change per update

// #define SERVO_PIN 5
// #define MOTOR_BASE_TIME 500    // Minimum pulse width (0° position)
// #define MOTOR_CHANGE_CONST 10  // µs per degree (2400-500)/180 ≈ 10.56
// #define SBUS_RX_PIN 18 // Change this to an available GPIO pin

// int currentAngle = 90;      // Starting angle
// int targetAngle = 90;       // Target angle
// unsigned long lastUpdate = 0;
// uint8_t servo_value = 127;

// // Min/Max Motor Speed for Debugger
// int max_motor_speed = 100;
// int min_motor_speed = -100;

// // Motor Speed Scalar
// int motor_speed_scalar = 0;

// // Bias for the Motor Speed
// int motor_speed_bias = 0;

// // Min/Max Servo Speed for Debugger
// int max_servo_speed = 100;
// int min_servo_speed = -100;


// // SBUS objects
// // bfs::SbusRx sbus_rx(&Serial2, SBUS_RX_PIN, -1, false); // Using external inverter
// // bfs::SbusData sbusData;

// void setup() {
//   Serial.begin(115200);
//   pinMode(SERVO_PIN, OUTPUT);
//   Serial.println("Servo gradual control test");
//   Serial.println("Commands: 'l' (left), 'c' (center), 'r' (right), '+'/'-' (adjust speed)");


//     // // Initialize SBUS on Serial2
//     // Serial2.begin(100000, SERIAL_8E2, SBUS_RX_PIN, -1);
//     // sbus_rx.Begin();
// }

// void control_motors_joystick(uint16_t ver_val, uint16_t hor_val) {
//     // These calculations might need to be
//     // changed based on calibrations!!
//     // center both values at 0
//     int16_t hor_val_origin = hor_val - 124;
//     int16_t ver_val_origin = ver_val - 124;
//     int16_t left_motor = ver_val_origin + hor_val_origin;
//     int16_t right_motor = ver_val_origin - hor_val_origin;
//     // The Clamping of Values is Now Done in the Motor Servo Control
//     // ver_val represents throttle, hor_val represents steering

//     int16_t left_speed = ((left_motor * 100) >> 9);
//     int16_t right_speed = ((right_motor * 100) >> 9);

//     // Clamp Speed if Less Than 3
//     if (abs(left_speed) < 3)
//       left_speed = 0;
//     if (abs(right_speed) < 3)
//       right_speed = 0;

//     // SerialUSB.print(left_speed);
//     // SerialUSB.print(" ");
//     // SerialUSB.print(right_speed);
//     // SerialUSB.print("\n");

//     change_motor_speed(0, left_speed);
// }

// // Sets Motor Speed
// void change_motor_speed(uint8_t motor_num, int16_t speed) {

//     // Apply the Bias
//     if (abs(speed) > 5)
//         speed += motor_speed_bias;

//     // Left Motor
//     if (!motor_num) {
//         // Calculate speed value target
//         servo_value = transformSpeed(speed, min_motor_speed, max_motor_speed, motor_speed_scalar);
//     }

//     // Right Motor
//     else {
//         // Calculate speed value
//         servo_value = 255 - transformSpeed(speed, min_motor_speed, max_motor_speed, motor_speed_scalar);
//     }
// }


// uint8_t transformSpeed(int16_t speed, int min_speed, int max_speed, int scalar) {

//     // Apply the Scalar
//     if (speed > 0) {
//         speed += scalar;
//         if (speed < 0)
//             speed = 0;
//     }
//     else {
//         speed -= scalar;
//         if (speed > 0)
//             speed = 0;
//     }

//     // Clamp Speed Between Min and Max Speed
//     if (speed > max_speed)
//         speed = max_speed;
//     else if (speed < min_speed)
//         speed = min_speed;

//     // Transform Value into an Unsigned Byte
//     return 127 - speed;
// }

// void sendServoPulse(int angle) {
//     // Calculate pulse width in microseconds
//     // int pulseWidth = MOTOR_BASE_TIME + (MOTOR_CHANGE_CONST * servo_value);
//     // int pulseWidth = map(servo_value, 88, 168, 0, 180);

//     // pulseWidth = constrain(pulseWidth, 500, 2400);

//     int pulseWidth = map(angle, 0, 180, 500, 2400);
//     // Serial.println(angle);
//     // Generate PWM pulse
//     digitalWrite(SERVO_PIN, HIGH);
//     delayMicroseconds(pulseWidth);
//     digitalWrite(SERVO_PIN, LOW);

//     // Maintain 20ms period (50Hz refresh rate)
//     // delayMicroseconds(20000 - pulseWidth);
// }

// void updateServo() {
//   unsigned long currentTime = millis();

//   // Send pulse regardless of angle change
//   sendServoPulse(currentAngle);

//   // Only update position at specified intervals
//   if (currentTime - lastUpdate >= SERVO_UPDATE_MS) {
//     lastUpdate = currentTime;

//     // Gradually move toward target
//     if (currentAngle < targetAngle) {
//       currentAngle += SERVO_STEP;
//     } else if (currentAngle > targetAngle) {
//       currentAngle -= SERVO_STEP;
//     }
//   }
// }


// void testServoSweep() {
//   Serial.println("Starting servo sweep test...");

//   // Move to left position
//   Serial.println("Moving to left position");
//   for(int angle = 90; angle >= 0; angle--) {
//     sendServoPulse(angle);
//     delay(20);
//   }
//   delay(1000);

//   // Sweep to right position
//   Serial.println("Sweeping to right position");
//   for(int angle = 0; angle <= 180; angle++) {
//     sendServoPulse(angle);
//     delay(20);
//   }
//   delay(1000);

//   // Return to center
//   Serial.println("Returning to center");
//   for(int angle = 180; angle >= 90; angle--) {
//     sendServoPulse(angle);
//     delay(20);
//   }

//   Serial.println("Test complete");
// }

// void loop() {
//   // Process commands
//   // if (Serial.available()) {
//   //   char cmd = Serial.read();
//   //   switch (cmd) {
//   //     case 'l': targetAngle = 0; break;    // Left
//   //     case 'c': targetAngle = 90; break;   // Center
//   //     case 'r': targetAngle = 180; break;  // Right
//   //     case '+': targetAngle += 10; break;  // Increment
//   //     case '-': targetAngle -= 10; break;  // Decrement
//   //   }
//   //   targetAngle = constrain(targetAngle, 0, 180);
//   //   Serial.print("Target angle: ");
//   //   Serial.println(targetAngle);
//   // }

//   // updateServo();
//   // delay(2); // Small delay for stability
//   uint16_t ver_val = 1024; // Replace with actual joystick value
//   uint16_t hor_val = 1024; // Replace with actual joystick value

//     //   static unsigned long lastFrameTime = 0;
//     // const int frameDelay = 1000 / 11; // ~91ms per frame
//     // unsigned long currentTime = millis();
//     // static unsigned long lastSbusUpdate = 0;
//     // const int sbusUpdateInterval = 100; // Update every 100ms to avoid flooding serial

//     // // Read and process SBUS data at a lower frequency
//     // if (currentTime - lastSbusUpdate >= sbusUpdateInterval) {
//     //     lastSbusUpdate = currentTime;

//     //   if (sbus_rx.Read()) {
//     //     sbusData = sbus_rx.data();

//     //     Serial.println(sbusData.ch[14]);
//     //     uint16_t ver_8bit = sbusData.ch[14] >> 3;
//     //     uint16_t hor_8bit = ver_8bit;

//     //     control_motors_joystick(ver_8bit, hor_8bit);

//     //     Serial.println("Servo number: ");
//     //     Serial.print(servo_value);

//     //     updateServo();
//     //   }
//     // }

//     testServoSweep();

//   // testServoSweep();
// }


// -------------------------- SBUS Library + SCREEN --------------------------

// #include <Arduino.h>
// #include "src/ScreenManager.h"
// #include "src/uart1_driver.h"
// #include <sbus.h>

// // Pin definitions
// #define TFT_CS   37
// #define TFT_DC   35
// #define TFT_RST  1
// #define TFT_LITE 2
// #define SBUS_RX_PIN 18 // Change this to an available GPIO pin

// // Create objects
// ScreenManager screenManager(TFT_CS, TFT_DC, TFT_RST, TFT_LITE);
// UART1Driver uart1;
// String receivedData = "";

// // SBUS objects
// bfs::SbusRx sbus_rx(&Serial2, SBUS_RX_PIN, -1, false); // Using external inverter
// bfs::SbusData sbusData;

// // Flag to track if we have SBUS data
// bool haveSbusData = false;

// void setup() {
//     Serial.begin(115200);
//     Serial.println("SBUS Test with Grid Integration");

//     // Initialize screen
//     screenManager.begin();

//     // Initialize UART1
//     uart1.init(115200);

//     // Initialize SBUS on Serial2
//     Serial2.begin(100000, SERIAL_8E2, SBUS_RX_PIN, -1);
//     sbus_rx.Begin();

//     Serial.println("SBUS initialized on pin " + String(SBUS_RX_PIN));
//     Serial.println("Waiting for SBUS data...");
// }

// void loop() {
//     static unsigned long lastFrameTime = 0;
//     const int frameDelay = 1000 / 11; // ~91ms per frame
//     unsigned long currentTime = millis();
//     static unsigned long lastSbusUpdate = 0;
//     const int sbusUpdateInterval = 100; // Update every 100ms

//     // Read and process SBUS data
//     if (currentTime - lastSbusUpdate >= sbusUpdateInterval) {
//         lastSbusUpdate = currentTime;

//         if (sbus_rx.Read()) {
//             sbusData = sbus_rx.data();
//             haveSbusData = true;

//             // Print SBUS data to serial monitor in a 4x4 grid format
//             Serial.println("\n=== SBUS Channel Values ===");

//             // Row 1: Channels 1-4
//             Serial.print("CH 1-4:   ");
//             for (int i = 0; i < 4; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();

//             // Row 2: Channels 5-8
//             Serial.print("CH 5-8:   ");
//             for (int i = 4; i < 8; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();

//             // Row 3: Channels 9-12
//             Serial.print("CH 9-12:  ");
//             for (int i = 8; i < 12; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();

//             // Row 4: Channels 13-16
//             Serial.print("CH 13-16: ");
//             for (int i = 12; i < 16; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();

//             // Status flags
//             Serial.print("Lost Frame: ");
//             Serial.print(sbusData.lost_frame ? "YES" : "NO");
//             Serial.print("\tFailsafe: ");
//             Serial.println(sbusData.failsafe ? "ACTIVE" : "OFF");
//             Serial.println("===========================");

//             // Update the grid with SBUS values
//             screenManager.displaySbusGrid(sbusData.ch);
//         }
//     }

//     // Update screen with normal grid if no SBUS data yet
//     if (!haveSbusData && currentTime - lastFrameTime >= frameDelay) {
//         lastFrameTime = currentTime;
//         screenManager.displayGrid(); // Use random values if no SBUS data yet
//     }

//     // UART processing
//     uint8_t* uartData = uart1.receive();
//     size_t len = strlen((char*)uartData);
//     if (uartData != nullptr && len > 0) {
//         Serial.print("Received from UART: ");
//         Serial.println((char*)uartData);
//         screenManager.displayUartData(uartData, len);
//     }

//     // Rest of your code for LED and serial monitor
//     pinMode(42, OUTPUT);
//     digitalWrite(42, HIGH);
//     delay(500);
//     digitalWrite(42, LOW);
//     delay(500);

//     while(Serial.available()) {
//         char incomingChar = Serial.read();
//         if (incomingChar == '\n') {
//             Serial.print("Received: ");
//             Serial.println(receivedData);
//             receivedData = "";
//         } else {
//             receivedData += incomingChar;
//         }
//     }
// }

// -------------------------- SBUS Library Implementation --------------------------


// #include <Arduino.h>
// #include "src/ScreenManager.h"
// #include "src/uart1_driver.h"
// #include <sbus.h>

// // Pin definitions
// #define TFT_CS   37
// #define TFT_DC   35
// #define TFT_RST  1
// #define TFT_LITE 2
// #define SBUS_RX_PIN 4 // Change this to an available GPIO pin

// // Create objects
// ScreenManager screenManager(TFT_CS, TFT_DC, TFT_RST, TFT_LITE);
// UART1Driver uart1;
// String receivedData = "";

// // SBUS objects
// bfs::SbusRx sbus_rx(&Serial2, SBUS_RX_PIN, -1, false); // Using external inverter
// bfs::SbusData sbusData;

// void setup() {
//     Serial.begin(115200);
//     Serial.println("SBUS Test - Terminal Output Only");

//     // Initialize screen
//     screenManager.begin();

//     // Initialize UART1
//     uart1.init(115200);

//     // Initialize SBUS on Serial2
//     Serial2.begin(100000, SERIAL_8E2, SBUS_RX_PIN, -1);
//     sbus_rx.Begin();

//     Serial.println("SBUS initialized on pin " + String(SBUS_RX_PIN));
//     Serial.println("Waiting for SBUS data...");
// }

// void loop() {
//     static unsigned long lastFrameTime = 0;
//     const int frameDelay = 1000 / 11; // ~91ms per frame
//     unsigned long currentTime = millis();
//     static unsigned long lastSbusUpdate = 0;
//     const int sbusUpdateInterval = 100; // Update every 100ms to avoid flooding serial

//     // Continue with your original screen update code
//     if (currentTime - lastFrameTime >= frameDelay) {
//         lastFrameTime = currentTime;
//         screenManager.displayGrid();
//     }

//     // Read and process SBUS data at a lower frequency
//     if (currentTime - lastSbusUpdate >= sbusUpdateInterval) {
//         lastSbusUpdate = currentTime;

//         if (sbus_rx.Read()) {
//             sbusData = sbus_rx.data();

//             // Print SBUS data to serial monitor in a 4x4 grid format
//             Serial.println("\n=== SBUS Channel Values ===");

//             // Row 1: Channels 1-4
//             Serial.print("CH 1-4:   ");
//             for (int i = 0; i < 4; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();

//             // Row 2: Channels 5-8
//             Serial.print("CH 5-8:   ");
//             for (int i = 4; i < 8; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();

//             // Row 3: Channels 9-12
//             Serial.print("CH 9-12:  ");
//             for (int i = 8; i < 12; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();

//             // Row 4: Channels 13-16
//             Serial.print("CH 13-16: ");
//             for (int i = 12; i < 16; i++) {
//                 Serial.print(sbusData.ch[i]);
//                 Serial.print("\t");
//             }
//             Serial.println();

//             // Status flags
//             Serial.print("Lost Frame: ");
//             Serial.print(sbusData.lost_frame ? "YES" : "NO");
//             Serial.print("\tFailsafe: ");
//             Serial.println(sbusData.failsafe ? "ACTIVE" : "OFF");
//             Serial.println("===========================");
//         }
//     }
// }


// -------------------------- DRIVER TESTING??? --------------------------

// #include <Arduino.h>
// #include "src/i2c_driver.h"
// #include "src/spi1_driver.h"
// #include "src/test_functions.h"

// SPI1Driver spidriver1;
// I2CDriver i2cdriver;

// void setup() {
//   Serial.begin(115200);
//   spidriver1.init(1000000);
// }

// void loop() {
//  spidriver1.test_spi_pins(38, 37, 39, 36);
// //  i2cdriver.test_i2c_pins(9, 8, 100000, 0x28);
//  test_pin_blink(1, 1000);
// }




