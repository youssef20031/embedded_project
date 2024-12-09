// servo.h

#ifndef SERVO_H
#define SERVO_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdint.h>

class MG90SServo {
private:
    uint32_t gpio_pin;
    uint pwm_slice;
    uint pwm_channel;
    uint32_t wrap; // Store wrap value

public:
    // Constructor to initialize the servo on a specific GPIO pin
    MG90SServo(uint32_t pin);

    // Method to set the servo position based on pulse width in microseconds
    void setPosition(uint32_t pulse_width_us);
};

#endif // SERVO_H