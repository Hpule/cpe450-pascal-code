#ifndef MOTOR_PWM_CONTROL_H
#define MOTOR_PWM_CONTROL_H

#include "driver/ledc.h"
#include "esp_timer.h"

#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

// LEDC channel and timer config
#define LEFT_MOTOR_CHANNEL   LEDC_CHANNEL_3
#define RIGHT_MOTOR_CHANNEL  LEDC_CHANNEL_1
#define DOME_SERVO_CHANNEL   LEDC_CHANNEL_2

#define LEFT_MOTOR_PIN       47
#define RIGHT_MOTOR_PIN      48
#define DOME_SERVO_PIN       46

#define PWM_FREQUENCY_HZ     50
#define PWM_RESOLUTION_BITS  LEDC_TIMER_12_BIT
#define PWM_TIMER            LEDC_TIMER_0
#define PWM_MODE             LEDC_LOW_SPEED_MODE
#define ACCEL_INTERVAL_US    25600

#ifdef __cplusplus
extern "C" {
#endif

void setup_pwm_outputs();
void setup_pwm();
void setup_acceleration_timer();

void update_motor_pwm(ledc_channel_t channel, uint8_t value);
void set_pwm_ms(ledc_channel_t channel, float ms_on_time);

void changeMotorAcceleration(unsigned int target, unsigned int* value, unsigned int* zero_timer, int motor_index);
void control_motors_joystick(uint16_t ver_val, uint16_t hor_val);
void change_motor_speed(uint8_t motor_num, int16_t speed);
uint8_t transformSpeed(int16_t speed, int min_speed, int max_speed, int scalar);
void set_servo_angle(int16_t speed);

extern unsigned int motor_value_left;
extern unsigned int motor_target_left;
extern unsigned int motor_zero_timer_left;

extern unsigned int motor_value_right;
extern unsigned int motor_target_right;
extern unsigned int motor_zero_timer_right;

extern int max_motor_speed;
extern int min_motor_speed;
extern int motor_speed_scalar;
extern int motor_speed_bias;

// servo value
extern unsigned int servo_value;

// Min/Max Servo Speed for Debugger
extern int max_servo_speed;
extern int min_servo_speed;

// Servo Speed Scalar
extern int servo_speed_scalar;

#ifdef __cplusplus
}
#endif

// Declare the ISR outside of extern "C"
void IRAM_ATTR acceleration_timer_callback(void* arg);

#endif // MOTOR_PWM_CONTROL_H
