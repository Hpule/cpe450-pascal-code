#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

#include <Arduino.h>

// test functions for testing the code

void test_pin_blink(int pin, int delay_time) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(delay_time);
    digitalWrite(pin, LOW);
    delay(delay_time);
}

#endif