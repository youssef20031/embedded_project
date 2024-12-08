// servo.cpp

#include "servo.h"
#include <stdio.h>

SG90Servo::SG90Servo(uint32_t pin) : gpio_pin(pin) {
    // Initialize GPIO for PWM
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    pwm_slice = pwm_gpio_to_slice_num(gpio_pin);
    pwm_channel = pwm_gpio_to_channel(gpio_pin);

    // Set PWM frequency to 50Hz for SG90
    uint32_t freq = 50;
    wrap = 1024;
    pwm_set_wrap(pwm_slice, wrap);
    // Calculate and set the clock divider
    float clock_divider = 125000000.0f / (freq * wrap); // 125MHz clock
    pwm_set_clkdiv(pwm_slice, clock_divider);

    // Initialize PWM to neutral position
    setPosition(512);
    pwm_set_enabled(pwm_slice, true);
}

void SG90Servo::setPosition(uint32_t position) {
    if (position > 1024) position = 1024;
    // Map 0-1024 to pulse width (0.5ms to 2.5ms)
    uint32_t pulse_width = (position * 2000) / 1024 + 500; // in microseconds
    uint32_t level = (pulse_width * wrap) / 20000; // 20ms period

    // Ensure level does not exceed wrap
    if (level > wrap) level = wrap;

    pwm_set_chan_level(pwm_slice, pwm_channel, level);
    printf("Servo position set to %u (Pulse Width: %u us)\n", position, pulse_width);
}