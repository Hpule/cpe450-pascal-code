// motor_pwm_control.cpp
#include "motor_pwm_control.h"

// Global timer handle
esp_timer_handle_t accel_timer;

// // Motor control state variables
// unsigned int motor_value_left = 127;
// unsigned int motor_target_left = 127;
// unsigned int motor_zero_timer_left = 0;

// unsigned int motor_value_right = 127;
// unsigned int motor_target_right = 127;
// unsigned int motor_zero_timer_right = 0;

// // Min/Max Motor Speed for Debugger
// int max_motor_speed = 100;
// int min_motor_speed = -100;

// // Motor Speed Scalar
// int motor_speed_scalar = 0;

// // Bias for the Motor Speed
// int motor_speed_bias = 0;

void setup_pwm_outputs() {
    ledc_timer_config_t timer_conf = {
        .speed_mode = PWM_MODE,
        .duty_resolution = PWM_RESOLUTION_BITS,
        .timer_num = PWM_TIMER,
        .freq_hz = PWM_FREQUENCY_HZ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t pwm_channels[] = {
        {
            .channel    = LEFT_MOTOR_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEFT_MOTOR_PIN,
            .speed_mode = PWM_MODE,
            .hpoint     = 0,
            .timer_sel  = PWM_TIMER
        },
        {
            .channel    = RIGHT_MOTOR_CHANNEL,
            .duty       = 0,
            .gpio_num   = RIGHT_MOTOR_PIN,
            .speed_mode = PWM_MODE,
            .hpoint     = 0,
            .timer_sel  = PWM_TIMER
        },
        {
            .channel    = DOME_SERVO_CHANNEL,
            .duty       = 0,
            .gpio_num   = DOME_SERVO_PIN,
            .speed_mode = PWM_MODE,
            .hpoint     = 0,
            .timer_sel  = PWM_TIMER
        }
    };

    for (int i = 0; i < sizeof(pwm_channels) / sizeof(pwm_channels[0]); i++) {
        ledc_channel_config(&pwm_channels[i]);
    }
}

void set_pwm_ms(ledc_channel_t channel, float ms_on_time) {
    uint32_t max_duty = (1 << PWM_RESOLUTION_BITS) - 1;
    float period_ms = 1000.0f / PWM_FREQUENCY_HZ;
    uint32_t duty = (ms_on_time / period_ms) * max_duty;
    ledc_set_duty(PWM_MODE, channel, duty);
    ledc_update_duty(PWM_MODE, channel);
}


void setup_acceleration_timer() {
    const esp_timer_create_args_t accel_timer_args = {
        .callback = &acceleration_timer_callback,
        .name = "accel_control"
    };
    esp_timer_create(&accel_timer_args, &accel_timer);
    esp_timer_start_periodic(accel_timer, ACCEL_INTERVAL_US);
}

void setup_pwm() {
    setup_pwm_outputs();
    setup_acceleration_timer();
}

void update_motor_pwm(ledc_channel_t channel, uint8_t value) {
    const float base_time_ms = 1.0f;
    const float change_per_unit_ms = 0.0078f;
    float ms_on_time = base_time_ms + (value * change_per_unit_ms);
    set_pwm_ms(channel, ms_on_time);
}

void changeMotorAcceleration(unsigned int target, unsigned int& value, unsigned int& zero_timer, int motor_index) {
    if (target == value) return;
    if (zero_timer != 0) {
        zero_timer--;
        return;
    }
    if (value < target) {
        value += 1;
    } else {
        value -= 1;
    }
    if (value == 127) zero_timer = 20;

    update_motor_pwm((motor_index == 0) ? LEFT_MOTOR_CHANNEL : RIGHT_MOTOR_CHANNEL, value);
}

void control_motors_joystick(uint16_t ver_val, uint16_t hor_val) {
    int16_t hor_val_origin = hor_val - 124;
    int16_t ver_val_origin = ver_val - 124;
    int16_t left_motor = ver_val_origin + hor_val_origin;
    int16_t right_motor = ver_val_origin - hor_val_origin;

    int16_t left_speed = ((left_motor * 100) >> 9);
    int16_t right_speed = ((right_motor * 100) >> 9);

    if (abs(left_speed) < 3) left_speed = 0;
    if (abs(right_speed) < 3) right_speed = 0;

    change_motor_speed(0, left_speed);
    change_motor_speed(1, right_speed);
}

void change_motor_speed(uint8_t motor_num, int16_t speed) {
    if (abs(speed) > 5)
        speed += motor_speed_bias;

    if (motor_num == 0) {
        motor_target_left = transformSpeed(speed, min_motor_speed, max_motor_speed, motor_speed_scalar);
    } else {
        motor_target_right = 255 - transformSpeed(speed, min_motor_speed, max_motor_speed, motor_speed_scalar);
    }
}

uint8_t transformSpeed(int16_t speed, int min_speed, int max_speed, int scalar) {
    if (speed > 0) {
        speed += scalar;
        if (speed < 0) speed = 0;
    } else {
        speed -= scalar;
        if (speed > 0) speed = 0;
    }
    if (speed > max_speed) speed = max_speed;
    else if (speed < min_speed) speed = min_speed;

    return 127 - speed;
}
