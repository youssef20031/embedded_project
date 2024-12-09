// servo.cpp

#include "servo.h"
#include <stdio.h>

MG90SServo::MG90SServo(uint32_t pin) : gpio_pin(pin) {
    // Initialize GPIO for PWM
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    pwm_slice = pwm_gpio_to_slice_num(gpio_pin);
    pwm_channel = pwm_gpio_to_channel(gpio_pin);

    // Set PWM frequency to 50Hz for MG90S
    uint32_t freq = 50;
    wrap = 20000; // 20ms period with 1MHz clock
    pwm_set_wrap(pwm_slice, wrap);

    // Set the clock divider to get a 1MHz PWM clock (125MHz / 125)
    float clock_divider = 125.0f;
    pwm_set_clkdiv(pwm_slice, clock_divider);

    // Initialize PWM to neutral position (1.5ms pulse width)
    setPosition(1500);
    pwm_set_enabled(pwm_slice, true);
}

void MG90SServo::setPosition(uint32_t pulse_width_us) {
    // Ensure pulse width is within valid range (500us to 2500us)
    if (pulse_width_us < 500) pulse_width_us = 500;
    if (pulse_width_us > 2500) pulse_width_us = 2500;

    // Set PWM channel level based on pulse width
    uint32_t level = pulse_width_us; // Because PWM clock is 1MHz

    pwm_set_chan_level(pwm_slice, pwm_channel, level);
    printf("Servo pulse width set to %u us\n", pulse_width_us);
}