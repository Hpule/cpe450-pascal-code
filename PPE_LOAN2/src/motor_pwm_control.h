#ifndef MOTOR_PWM_CONTROL_H
#define MOTOR_PWM_CONTROL_H

#include "driver/ledc.h"
#include "esp_timer.h"

#define LEFT_MOTOR_CHANNEL   0
#define RIGHT_MOTOR_CHANNEL  1
#define DOME_SERVO_CHANNEL   2

#define LEFT_MOTOR_PIN       47
#define RIGHT_MOTOR_PIN      48
#define DOME_SERVO_PIN       46

#define PWM_FREQUENCY_HZ     50
#define PWM_RESOLUTION_BITS  12
#define PWM_TIMER            LEDC_TIMER_0
#define PWM_MODE             LEDC_LOW_SPEED_MODE
#define ACCEL_INTERVAL_US    25600  // ~25.6ms

#ifdef __cplusplus
extern "C" {
#endif

// Setup functions
void setup_pwm_outputs();
void setup_pwm();
void setup_acceleration_timer();

// PWM control
void update_motor_pwm(uint8_t channel, uint8_t value);
void set_pwm_ms(uint8_t channel, float ms_on_time);

// Motor control and acceleration
void changeMotorAcceleration(unsigned int target, unsigned int* value, unsigned int* zero_timer, int motor_index);
void control_motors_joystick(uint16_t ver_val, uint16_t hor_val);
void change_motor_speed(uint8_t motor_num, int16_t speed);
uint8_t transformSpeed(int16_t speed, int min_speed, int max_speed, int scalar);

// // Motor configuration state
// extern unsigned int motor_value_left;
// extern unsigned int motor_target_left;
// extern unsigned int motor_zero_timer_left;

// extern unsigned int motor_value_right;
// extern unsigned int motor_target_right;
// extern unsigned int motor_zero_timer_right;

// extern int max_motor_speed;
// extern int min_motor_speed;
// extern int motor_speed_scalar;
// extern int motor_speed_bias;

// Motor control state variables
unsigned int motor_value_left = 127;
unsigned int motor_target_left = 127;
unsigned int motor_zero_timer_left = 0;

unsigned int motor_value_right = 127;
unsigned int motor_target_right = 127;
unsigned int motor_zero_timer_right = 0;

// Min/Max Motor Speed for Debugger
int max_motor_speed = 100;
int min_motor_speed = -100;

// Motor Speed Scalar
int motor_speed_scalar = 0;

// Bias for the Motor Speed
int motor_speed_bias = 0;

#ifdef __cplusplus
}
#endif

// Timer interrupt should be outside extern "C"
// void IRAM_ATTR acceleration_timer_callback(void* arg);

#endif // MOTOR_PWM_CONTROL_H
