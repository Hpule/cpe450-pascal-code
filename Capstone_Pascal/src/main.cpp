#include <Arduino.h>
#define LED_RGB 2

// put function declarations here:
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RGB, OUTPUT);
  Serial.begin(9600);
  Serial.println("Setup function");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  digitalWrite(LED_RGB, HIGH);
  Serial.println("Loop function");
  delay(1000);
  digitalWrite(LED_RGB, LOW);
}

